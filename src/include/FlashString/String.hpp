/****
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

// Wiring String - this file is included from WString.h so define required types only
class String;
class __FlashStringHelper;
typedef const __FlashStringHelper* flash_string_t;

/**
 * @defgroup fstr_string Strings
 * @ingroup FlashString
 * @{
 */

/**
 * @brief Define an inline String and return a pointer to it
 * @note The rather obscure `asm` statement is required to prevent the compiler from discarding
 * the symbol at link time, which leads to an 'undefined reference' error
 */
#define FS_PTR(str)                                                                                                    \
	(__extension__({                                                                                                   \
		static DEFINE_FSTR_DATA(__fstr__, str);                                                                        \
		static_cast<const FSTR::String*>(&__fstr__.object);                                                            \
	}))

/**
 * @brief Define an inline FSTR::String and return it as a copy
 *
 * Example:
 *
 * 		Serial.println(FS("This is a Flash String"));
 */
#define FS(str) *FS_PTR(str)

/**
 * @brief Declare a global FSTR::String& reference
 * @param name
 * @note Define the FSTR::String object using DEFINE_STR()
 */
#define DECLARE_FSTR(name) extern const FSTR::String& name;

/**
 * @brief Define a FSTR::String object with global reference
 * @param name Name of FSTR::String& reference to define
 * @param str Content of the FSTR::String
 *
 *	Example:
 *
 *		DEFINE_FSTR(test, "This is a test\0Another test\0hello")
 *
 * The data includes the nul terminator but the length does not.
 *
 */
#define DEFINE_FSTR(name, str)                                                                                         \
	static DEFINE_FSTR_DATA(FSTR_DATA_NAME(name), str);                                                                \
	DEFINE_FSTR_REF_NAMED(name, FSTR::String);

/**
 * @brief Define a FSTR::String object with local reference
 * @param name Name of FSTR::String& reference to define
 * @param str Content of the FSTR::String
 */
#define DEFINE_FSTR_LOCAL(name, str)                                                                                   \
	static DEFINE_FSTR_DATA(FSTR_DATA_NAME(name), str);                                                                \
	static constexpr DEFINE_FSTR_REF_NAMED(name, FSTR::String);

/**
 * @brief Define a FSTR::String data structure
 * @param name Name of data structure
 * @param str Quoted string content
 */
#define DEFINE_FSTR_DATA(name, str)                                                                                    \
	constexpr const struct {                                                                                           \
		FSTR::ObjectBase object;                                                                                       \
		char data[ALIGNUP(sizeof(str))];                                                                               \
	} name PROGMEM = {{sizeof(str) - 1}, str};                                                                         \
	FSTR_CHECK_STRUCT(name);

/**
 * @brief Load a FSTR::String object into a named local (stack) buffer
 *
 * Example:
 *
 *		DEFINE_FSTR(globalTest, "This is a testing string")
 *		...
 *		LOAD_FSTR(local, globalTest)
 *		printf("%s, %u characters, buffer is %u bytes\n", local, globalTest.length(), sizeof(local));
 *
 */
#define LOAD_FSTR(name, fstr)                                                                                          \
	char name[(fstr).size()] FSTR_ALIGNED;                                                                             \
	memcpy_aligned(name, (fstr).data(), (fstr).length());                                                              \
	name[(fstr).length()] = '\0';

/**
 * @brief Define a flash FSTR::String and load it into a named char[] buffer on the stack
 * @param name Name of char[] buffer
 * @param str Content of FSTR::String
 * @note Equivalent to `char name[] = "text"` except the buffer is word aligned.
 * Faster than using a temporary Wiring ::String and avoids using the heap.
 */
#define FSTR_ARRAY(name, str)                                                                                          \
	static DEFINE_FSTR_DATA(FSTR_DATA_NAME(name), str);                                                                \
	LOAD_FSTR(name, FSTR_DATA_NAME(name).object.template as<FSTR::String>())

/**
 * @brief Define a FSTR::String containing data from an external file
 * @param name Name for the FSTR::String object
 * @param file Absolute path to the file containing the content
 */
#define IMPORT_FSTR(name, file)                                                                                        \
	IMPORT_FSTR_DATA(name, file)                                                                                       \
	extern "C" const FSTR::String name;

/**
 * @brief declare a table of FlashStrings
 * @param name name of the table
 * @deprecated Use a Vector or Map
 *
 * Declares a simple table. Example:
 *
 *  	DEFINE_FSTR(fstr1, "Test string #1");
 *  	DEFINE_FSTR(fstr2, "Test string #2");
 *
 *  	FSTR_TABLE(table) = {
 *  		&fstr1,
 *  		&fstr2,
 *  	};
 *
 * Table entries may be accessed directly as they are word-aligned. Examples:
 *
 *  	debugf("fstr1 = '%s'", FSTR::String(*table[0]).c_str());
 *  	debugf("fstr2.length() = %u", table[1]->length());
 *
 */
#define FSTR_TABLE(name) const FSTR::String* const name[] PROGMEM

namespace FSTR
{
/**
 * @brief A Wiring ::String
 */
typedef ::String WString;

/**
 * @brief describes a counted string stored in flash memory
 */
class String : public Object<String, char>
{
public:
	/**
	 * @brief Get the number of bytes used to store the String
	 * @note Always an integer multiple of 4 bytes
	 */
	size_t size() const
	{
		return ALIGNUP(Object::length() + 1);
	}

	/**
	 * @brief Get a WString-compatible pointer to the flash data
	 */
	flash_string_t data() const
	{
		return reinterpret_cast<flash_string_t>(Object::data());
	}

	/**
	 * @brief Check for equality with a C-string
	 * @param cstr
	 * @param len Length of cstr (optional)
	 * @retval bool true if strings are identical
	 * @note loads string into a stack buffer for the comparison, no heap required
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
	 *
	 * Avoids implicit String() cast when working with large FlashStrings:
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

/** @} */
