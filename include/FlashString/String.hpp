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
			FSTR::ObjectBase object;                                                                                   \
			char data[ALIGNUP(sizeof(str))];                                                                           \
		} struc PROGMEM = {{sizeof(str) - 1}, str};                                                                    \
		&struc.object.as<FSTR::String>();                                                                              \
	}))

/**
 * @brief Declare a global String instance
 */
#define DECLARE_FSTR(name) extern const FSTR::String& name;

/** @brief Define a String
 *  @param name variable to identify the string
 *  @param str content of the string
 *  @note the whole thing is word-aligned
 *  Example: DEFINE_FSTR(test, "This is a test\0Another test\0hello")
 *  The data includes the nul terminator but the length does not.
 */
#define DEFINE_FSTR(name, str)                                                                                         \
	static DEFINE_FSTR_DATA(FSTR_DATA_NAME(name), str);                                                                \
	DEFINE_FSTR_REF_NAMED(name, FSTR::String);

/** @brief Define a String for local (static) use
 *  @param name variable to identify the string
 *  @param str content of the string
 */
#define DEFINE_FSTR_LOCAL(name, str)                                                                                   \
	DEFINE_FSTR_DATA_LOCAL(FSTR_DATA_NAME(name), str);                                                                 \
	static DEFINE_FSTR_REF_NAMED(name, FSTR::String);

/** @brief Define a string in a String-compatible structure
 *  @param name Name to use for data structure
 *  @param str String to store
 */
#define DEFINE_FSTR_DATA(name, str)                                                                                    \
	constexpr const struct {                                                                                           \
		FSTR::ObjectBase object;                                                                                       \
		char data[ALIGNUP(sizeof(str))];                                                                               \
	} name PROGMEM = {{sizeof(str) - 1}, str};                                                                         \
	FSTR_CHECK_STRUCT(name);

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
	LOAD_FSTR(name, FSTR_DATA_NAME(name).object.as<FSTR::String>())

/** @brief Define a String containing data from an external file
 *  @param name Name for the String object
 *  @param file Absolute path to the file containing the content
 */
#define IMPORT_FSTR(name, file)                                                                                        \
	IMPORT_FSTR_DATA(name, file)                                                                                       \
	extern "C" const FSTR::String name;

namespace FSTR
{
using WString = ::String;

/**
 * @brief describes a counted string stored in flash memory
 */
class String : public Object<String, char>
{
public:
	/**
	 * @brief Get a WString-compatible pointer to the flash data
	 */
	flash_string_t data() const
	{
		return reinterpret_cast<flash_string_t>(&flashLength + 1);
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
};

} // namespace FSTR
