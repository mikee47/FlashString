/**
 * String.h - Defines the String class and associated macros for efficient flash memory string access.
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

#include <WString.h>
#include <FakePgmSpace.h>

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
	const FSTR::String& name PROGMEM = FSTR_DATA_NAME(name).fstr;

/** @brief Define a String for local (static) use
 *  @param name variable to identify the string
 *  @param str content of the string
 */
#define DEFINE_FSTR_LOCAL(name, str)                                                                                   \
	DEFINE_FSTR_DATA_LOCAL(FSTR_DATA_NAME(name), str);                                                                 \
	static const FSTR::String& name PROGMEM = FSTR_DATA_NAME(name).fstr;

/**
 * @brief Cast a pointer to String*
 * @param data_ptr Pointer to aligned structure with first word as length
 * @note Use if necessary for custom structures
 */
#define FSTR_PTR(data_ptr) reinterpret_cast<const FSTR::String*>(data_ptr)

/**
 * @brief Define a String& reference to a structure using a cast
 * @param name Name of the reference variable
 * @param data_name Name of structure to be referenced, in PROGMEM and word-aligned. First element MUST be the length.
 * @note Use to cast custom data structures into String format.
 */
#define DEFINE_FSTR_REF(name, data) const FSTR::String& name = *FSTR_PTR(&data);

/**
 * @brief Provide internal name for generated flash string structures
 */
#define FSTR_DATA_NAME(name) fstr_data_##name

/** @brief Define a string in a String-compatible structure
 *  @param name Name to use for data structure
 *  @param str ::String to store
 */
#define DEFINE_FSTR_DATA(name, str)                                                                                    \
	constexpr struct {                                                                                                 \
		FSTR::String fstr;                                                                                             \
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
	LOAD_FSTR(name, FSTR_DATA_NAME(name).fstr)

/** @brief Define a String containing data from an external file
 *  @param name Name for the String object
 *  @param file Path to the file to be included. This should be an absolute path.
 *  @note This provides a more efficient way to read constant (read-only) file data.
 *  The file content is bound into firmware image at link time.
 *  @note The String object must be referenced or the linker won't emit it.
 *  @note Use PROJECT_DIR to locate files in your project's source tree. For example:
 *  		IMPORT_FSTR(myFlashString, PROJECT_DIR "/files/my_flash_file.txt");
 *  Use COMPONENT_PATH within a component.
 */
#define IMPORT_FSTR(name, file)                                                                                        \
	IMPORT_FSTR_DATA(name, file)                                                                                       \
	extern "C" const FSTR::String name;

/**
 * @brief Link the contents of a file and define a global symbol
 * @note We need inline assembler's `.incbin` instruction to actually import the data.
 * We use a macro STR() so that if required the name can be resolved from a #defined value.
 */
#define STR(x) XSTR(x)
#define XSTR(x) #x
#ifdef __WIN32
#define IMPORT_FSTR_DATA(name, file)                                                                                   \
	__asm__(".section .rodata\n"                                                                                       \
			".global _" STR(name) "\n"                                                                                 \
			".def _" STR(name) "; .scl 2; .type 32; .endef\n"                                                          \
			".align 4\n"                                                                                               \
			"_" STR(name) ":\n"                                                                                        \
			".long _" STR(name) "_end - _" STR(name) " - 4\n"                                                          \
			".incbin \"" file "\"\n"                                                                                   \
			"_" STR(name) "_end:\n");
#else
#ifdef ARCH_HOST
#define IROM_SECTION ".rodata"
#else
#define IROM_SECTION ".irom0.text"
#endif
#define IMPORT_FSTR_DATA(name, file)                                                                                   \
	__asm__(".section " IROM_SECTION "\n"                                                                              \
			".global " STR(name) "\n"                                                                                  \
			".type " STR(name) ", @object\n"                                                                           \
			".align 4\n" STR(name) ":\n"                                                                               \
			".long _" STR(name) "_end - " STR(name) " - 4\n"                                                           \
			".incbin \"" file "\"\n"                                                                                   \
			"_" STR(name) "_end:\n");
#endif

namespace FSTR
{
/**
 * @brief describes a counted string stored in flash memory
 */
class String
{
public:
	static const String& empty()
	{
		static const String empty_{0};
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
	 * @bytesToRead How many bytes to read (e.g. size of buffer)
	 * @readCache true to read data via CPU data cache, false to bypass and read flash directly
	 * @retval size_t Number of bytes actually read, may be less than bufSize
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
	 * @bytesToRead How many bytes to read (e.g. size of buffer)
	 * @retval size_t Number of bytes actually read, may be less than bufSize
	 * @note PROGMEM data is accessed via the CPU data cache, so to avoid degrading performance
	 * you can use this method to read data directly from flash memory.
	 * This is appropriate for infrequently accessed data, especially if it is large.
	 * For example, if storing content using `IMPORT_FSTR` instead of SPIFFS then it
	 * is generally better to avoid contaminating the cache.
	 * @see See also `FlashMemoryStream` class.
	 */
	size_t readFlash(size_t offset, void* buffer, size_t bytesToRead) const;

	/** @brief Check for equality with a C-string
	 *  @param cstr
	 *  @param len Length of cstr (optional)
	 *  @retval bool true if strings are identical
	 *  @note loads string into a stack buffer for the comparison, no heap required
	 */
	bool isEqual(const char* cstr, size_t len = 0) const;

	/** @brief Check for equality with another String
	 *  @param str
	 *  @retval bool true if strings are identical
	 *  @{
	 */
	bool isEqual(const String& str) const;

	bool isEqual(const ::String& str) const
	{
		return str.equals(*this);
	}
	/** @} */

	operator ::String() const
	{
		return ::String(data(), length());
	}

	bool operator==(const char* str) const
	{
		return isEqual(str);
	}

	bool operator==(const String& str) const
	{
		return isEqual(str);
	}

	bool operator==(const ::String& str) const
	{
		return isEqual(str);
	}

	bool operator!=(const char* str) const
	{
		return !isEqual(str);
	}

	bool operator!=(const String& str) const
	{
		return !isEqual(str);
	}

	bool operator!=(const ::String& str) const
	{
		return !isEqual(str);
	}

	uint32_t flashLength; ///< Number of bytes/characters in data
						  // const uint8_t data[]
};

} // namespace FSTR
