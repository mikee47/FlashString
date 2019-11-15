/**
 * String.hpp - Defines the String class and associated macros for efficient flash memory string access.
 *
 * Copyright 2019 mikee47 <mike@sillyhouse.net>
 *
 * This file is part of the FlashString Library
 *
 * This library is free software: you can redistribute it and/or modify it under the terms of the
 * GNU General Public License as published by the Free Software Foundation, version 3 or later.
 *
 * This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with FlashString.
 * If not, see <https://www.gnu.org/licenses/>.
 *
 * @author: 2018 - Mikee47 <mike@sillyhouse.net>
 *
 ****/

#pragma once

#include "Object.hpp"
#include "ArrayIterator.hpp"
#include "StringPrinter.hpp"

// Arduino String - cannot include WString.h here because of recursion
class String;
class __FlashStringHelper;
typedef const __FlashStringHelper* flash_string_t;

using FlashString = FSTR::String;

/**
 * @brief Define an inline FlashString
 * @note This returns a pointer, not a reference
 */
#define FS(str)                                                                                                        \
	(__extension__({                                                                                                   \
		static constexpr struct {                                                                                      \
			FlashString object;                                                                                        \
			char data[ALIGNUP(sizeof(str))];                                                                           \
		} struc PROGMEM = {{sizeof(str) - 1}, str};                                                                    \
		&struc.object;                                                                                                 \
	}))

/**
 * @brief Declare a global String instance
 */
#define DECLARE_FSTR(name) extern const FlashString& name;

/** @brief Define a String
 *  @param name variable to identify the string
 *  @param str content of the string
 *  @note the whole thing is word-aligned
 *  Example: DEFINE_FSTR(test, "This is a test\0Another test\0hello")
 *  The data includes the nul terminator but the length does not.
 */
#define DEFINE_FSTR(name, str)                                                                                         \
	static DEFINE_FSTR_DATA(FSTR_DATA_NAME(name), str);                                                                \
	DEFINE_FSTR_REF(name);

/** @brief Define a String for local (static) use
 *  @param name variable to identify the string
 *  @param str content of the string
 */
#define DEFINE_FSTR_LOCAL(name, str)                                                                                   \
	DEFINE_FSTR_DATA_LOCAL(FSTR_DATA_NAME(name), str);                                                                 \
	DEFINE_FSTR_REF_LOCAL(name);

/** @brief Define a string in a String-compatible structure
 *  @param name Name to use for data structure
 *  @param str String to store
 */
#define DEFINE_FSTR_DATA(name, str)                                                                                    \
	constexpr const struct {                                                                                           \
		FlashString object;                                                                                            \
		char data[ALIGNUP(sizeof(str))];                                                                               \
	} name PROGMEM = {{sizeof(str) - 1}, str};

#define DEFINE_FSTR_DATA_LOCAL(name, str) static DEFINE_FSTR_DATA(name, str)

/**
 * @brief Load a String object into a named local (stack) buffer
 * @note Example:
 *
 * 	DEFINE_FSTR(globalTest, "This is a testing string")
 * 	...
 * 	LOAD_FSTR(local, globalTest)
 * 	printf("%s, %u characters, buffer is %u bytes\n", local, globalTest.length(), sizeof(local));
 */
#define LOAD_FSTR(name, fstr)                                                                                          \
	char name[(fstr).size()] __attribute__((aligned(4)));                                                              \
	memcpy_aligned(name, (fstr).data(), (fstr).length());                                                              \
	name[(fstr).length()] = '\0';

/**
 * @brief Define a flash string and load it into a named char[] buffer on the stack
 * @note Equivalent to `char name[] = "text"` except the buffer is word aligned.
 */
#define FSTR_ARRAY(name, str)                                                                                          \
	DEFINE_FSTR_DATA_LOCAL(FSTR_DATA_NAME(name), str);                                                                 \
	LOAD_FSTR(name, FSTR_DATA_NAME(name).object)

/** @brief Define a String containing data from an external file
 *  @param name Name for the String object
 *  @param file Absolute path to the file containing the content
 */
#define IMPORT_FSTR(name, file)                                                                                        \
	IMPORT_FSTR_DATA(name, file)                                                                                       \
	extern "C" const FlashString name;

namespace FSTR
{
using WString = ::String;

/**
 * @brief describes a counted string stored in flash memory
 */
class String
{
public:
	using Iterator = ArrayIterator<String, char>;

	Iterator begin() const
	{
		return Iterator(*this, 0);
	}

	Iterator end() const
	{
		return Iterator(*this, length());
	}

	static const String& empty()
	{
		static const String PROGMEM empty_{0};
		return empty_;
	}

	/**
	 * @brief Get the length of the string in characters, excluding NUL terminator
	 */
	uint32_t length() const
	{
		return flashLength;
	}

	/**
	 * @brief Get the number of bytes used to store the String
	 * @note Always an integer multiple of 4 bytes
	 */
	uint32_t size() const
	{
		return ALIGNUP(flashLength + 1);
	}

	char valueAt(unsigned index) const
	{
		if(index >= length()) {
			return '\0';
		}

		auto c = pgm_read_byte(reinterpret_cast<const uint8_t*>(data()) + index);
		return static_cast<char>(c);
	}

	template <class ObjectType> const ObjectType& as() const
	{
		return *reinterpret_cast<const ObjectType*>(this);
	}

	/**
	 * @brief Array operator[]
	 */
	char operator[](unsigned index) const
	{
		return valueAt(index);
	}

	/**
	 * @brief Get a pointer to the flash data
	 */
	flash_string_t data() const
	{
		return reinterpret_cast<flash_string_t>(&flashLength + 1);
	}

	/**
	 * @brief Read contents of a String into RAM
	 * @param offset Zero-based offset from start of flash data to start reading
	 * @param buffer Where to store data
	 * @param bytesToRead How many bytes to read (e.g. size of buffer)
	 * @retval size_t Number of bytes actually read
	 */
	size_t read(size_t offset, void* buffer, size_t bytesToRead) const
	{
		if(offset >= flashLength) {
			return 0;
		}

		auto count = std::min(flashLength - offset, bytesToRead);
		memcpy_P(buffer, reinterpret_cast<const uint8_t*>(data()) + offset, count);
		return count;
	}

	/**
	 * @brief Read contents of a String into RAM, using flashread()
	 * @param offset Zero-based offset from start of flash data to start reading
	 * @param buffer Where to store data
	 * @param bytesToRead How many bytes to read (e.g. size of buffer)
	 * @retval size_t Number of bytes actually read
	 * @note PROGMEM data is accessed via the CPU data cache, so to avoid degrading performance
	 * you can use this method to read data directly from flash memory.
	 * This is appropriate for infrequently accessed data, especially if it is large.
	 * For example, if storing content using `IMPORT_FSTR` instead of SPIFFS then it
	 * is generally better to avoid contaminating the cache.
	 * @see See also `FlashMemoryStream` class.
	 */
	size_t readFlash(size_t offset, void* buffer, size_t bytesToRead) const
	{
		if(offset >= flashLength) {
			return 0;
		}

		auto count = std::min(flashLength - offset, bytesToRead);
		return readFlashData(buffer, reinterpret_cast<const uint8_t*>(data()) + offset, count);
	}

	/** @brief Check for equality with a C-string
	 *  @param cstr
	 *  @param len Length of cstr (optional)
	 *  @retval bool true if strings are identical
	 *  @note loads string into a stack buffer for the comparison, no heap required
	 */
	bool equals(const char* cstr, size_t len = 0) const;

	/** @brief Check for equality with another String
	 *  @param str
	 *  @retval bool true if strings are identical
	 */
	bool equals(const String& str) const;

	bool operator==(const char* str) const
	{
		return equals(str);
	}

	bool operator==(const String& str) const
	{
		return equals(str);
	}

	bool operator!=(const char* str) const
	{
		return !equals(str);
	}

	bool operator!=(const String& str) const
	{
		return !equals(str);
	}

	/* WString support */

	operator WString() const;

	bool equals(const WString& str) const;

	bool equalsIgnoreCase(const WString& str) const;

	bool operator==(const WString& str) const
	{
		return equals(str);
	}

	bool operator!=(const WString& str) const
	{
		return !equals(str);
	}

	/* Arduino Print support */

	/**
	 * @brief Supports printing of large String objects
	 * @note Avoids implicit String() cast when working with large FlashStrings:
	 *
	 *		IMPORT_FSTR(largeString, PROJECT_DIR "/files/large-text.txt");
	 * 		Serial.println(largeString.printer());
	 *
	 */
	StringPrinter printer() const
	{
		return StringPrinter(*this);
	}

	size_t printTo(Print& p) const
	{
		return printer().printTo(p);
	}

	/* Private member data */

	uint32_t flashLength;
	// const uint8_t data[]
};

} // namespace FSTR
