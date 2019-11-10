/****
 * Sming Framework Project - Open Source framework for high efficiency native ESP8266 development.
 * Created 2015 by Skurydin Alexey
 * http://github.com/SmingHub/Sming
 * All files of the Sming Core are provided under the LGPL v3 license.
 *
 * FlashString.h - Defines the FlashString class and associated macros for efficient flash memory string access.
 *
 * @author: 2018 - Mikee47 <mike@sillyhouse.net>
 *
 ****/

/*
 * Note on storing strings in flash memory
 *
 * A string literal (e.g. "string") used in code gets emitted to the .rodata segment by the compiler.
 * That means it gets read into RAM at startup and remains there.
 * To avoid this, and reclaim the RAM, the data must be stored in a different segment. This is done
 * using the PROGMEM macro, defined in FakePgmSpace.h.
 *
 * Once in flash memory, string data must be read into RAM before it can be used. Accessing the flash
 * memory directly is awkard. If locations are not strictly accessed as 4-byte words the system will probably
 * crash; I say 'probably' because sometimes it just behaves weirdly if the RAM address isn't aligned.
 *
 * `FakePgmSpace.h` provides the basic mechanisms for storing and reading flash strings, including
 * general-purpose string library functions.  These are well-documented Arduino-compatible routines.
 * Some additions have been made to Sming to cater for the ESP8266 use of these strings.
 *
 * 	F(string_literal) - loads a String object with the given text, which is allocated to flash.
 *		String s = F("test");
 *
 *	_F(string_literal) - Like F() except buffer is allocated on stack. Most useful where nul-terminated data is required.
 *		m_printf(_F("C-style string\n"));
 *
 *	DEFINE_PSTR(name, str) - declares a PSTR stored in flash. The variable (name) points to flash
 *	memory so must be accessed using the appropriate xxx_P function.
 *
 *	LOAD_PSTR(name, flash_str) - loads pre-defined PSTR into buffer on stack
 *		DEFINE_PSTR_LOCAL(testFlash, "This is a test string\n"); // Function scope requires static allocation
 *		LOAD_PSTR(test, testFlash)
 *		m_printf(test);
 *
 *	PSTR_ARRAY(name, str) - creates and loads string into named stack buffer
 *	Ensures loaded string stays in scope, unlike _F()
 *		String testfunc() {
 *			PSTR_ARRAY(test, "This is the test string");
 *			m_printf(test);
 *			...
 *      	return test;
 *      }
 *
 *  IMPORT_FSTR(name, file) - binds a file into the firmware image as a FlashString object
 *  This needs to be used at file scope, example:
 *  	IMPORT_FSTR(myFlashData, PROJECT_DIR "/files/myFlashData.bin")
 *
 *  Note the use of PROJECT_DIR to locate the file using an absolute path.
 *  If using this within a component, use COMPONENT_PATH instead.
 *
 *	Both DEFINE_PSTR and PSTR_ARRAY load a PSTR into a stack buffer, but using sizeof() on that buffer will return
 *	a larger value than the string itself because it's aligned. Calling sizeof() on the original flash data will
 *	get the right value. If it's a regular nul-terminated string then strlen_P() will get the length, although it's
 *	time-consuming.
 *
 *	FlashString
 *
 *		A 'counted string' defined as a C++ structure with a length field.
 *		Requires only a single flash read to get the length; no strlen_P, etc.
 *		Stores arbitrary data including nuls.
 *		A global instance can be shared amongst modules with no loss of efficiency.
 *		The data is word-aligned so reading is as fast as possible; there are no alignment issues to deal with.
 *		Structure-based, so can add methods to the structure instead of using macros.
 *		Strongly typed, so support can be added to other modules for implicit conversions, etc. (e.g. ArduinoJson)
 *		Works with updated String class to provide the simplest string/data management.
 *
 *		Macros are consistent with the PSTR types, but named 'FSTR'.
 *
 *  Notes on usage
 *
 *  	Best practice is usually to define constant data at the top of a module. Non-trivial strings are no different.
 *  	The mechanisms offered here provide a way to help do that.
 *
 */

#pragma once

#include "WString.h"
#include "FakePgmSpace.h"
#include <stringutil.h>

/**
 * @brief Provide internal name for generated flash string structures, which are cast to a `FlashString` reference
 */
#define FSTR_DATA_NAME(name) fstr_data_##name

/** @brief Define a FlashString
 *  @param name variable to identify the string
 *  @param str content of the string
 *  @note the whole thing is word-aligned
 *  Example: DEFINE_FSTR(test, "This is a test\0Another test\0hello")
 *  The data includes the nul terminator but the length does not.
 */
#define DEFINE_FSTR(name, str)                                                                                         \
	DEFINE_FSTR_DATA(FSTR_DATA_NAME(name), str);                                                                       \
	DEFINE_FSTR_ALIAS(name, FSTR_DATA_NAME(name));

/** @brief Define a FlashString for local (static) use
 *  @param name variable to identify the string
 *  @param str content of the string
 */
#define DEFINE_FSTR_LOCAL(name, str) \
		DEFINE_FSTR_DATA_LOCAL(FSTR_DATA_NAME(name), str); \
		DEFINE_FSTR_REF(name, FSTR_DATA_NAME(name));

/** @brief Define a string in a FlashString-compatible structure
 *  @param name Name of FlashString - structure will use this as a base for its own name
 *  @param str String to store
 */
#define DEFINE_FSTR_DATA(name, str)                                                                                    \
	extern "C" const struct {                                                                                          \
		uint32_t length;                                                                                               \
		char data[ALIGNUP(sizeof(str))];                                                                               \
	} name PROGMEM = {sizeof(str) - 1, str};

#define DEFINE_FSTR_DATA_LOCAL(name, str)                                                                              \
	static const struct {                                                                                              \
		uint32_t length;                                                                                               \
		char data[ALIGNUP(sizeof(str))];                                                                               \
	} name PROGMEM = {sizeof(str) - 1, str};

/**
 * @brief Declare a global FlashString instance
 */
#define DECLARE_FSTR(name) extern "C" const FlashString name;

/**
 * @brief Cast a pointer to FlashString*
 * @param data_ptr Pointer to aligned structure with first word as length
 */
#define FSTR_PTR(data_ptr) reinterpret_cast<const FlashString*>(data_ptr)

/**
 * @brief Define a FlashString reference
 * @param name Name of the reference variable
 * @param data_name Name of structure to be referenced, in PROGMEM and word-aligned. First element MUST be the length.
 * @note Use to cast custom data structures into FlashString format.
 */
#define DEFINE_FSTR_REF(name, data) const FlashString& name = *FSTR_PTR(&data);

#define MACROQUOT(x) #x
#define MACROQUOTE(x) MACROQUOT(x)

/**
 * @brief Define a FlashString alias
 * @param name Name of the FlashString variable to define
 * @param data_name Name of structure to be aliased, in PROGMEM and word-aligned. First element MUST be the length.
 * @note Allows a structure which isn't a FlashString to be accessed directly as if it were one
 */
#define DEFINE_FSTR_ALIAS(name, data_name)                                                                             \
	extern "C" const FlashString __attribute__((alias(MACROQUOTE(data_name)))) name;

/**
 * @brief Load a FlashString object into a named local (stack) buffer
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
	LOAD_FSTR(name, *FSTR_PTR(&FSTR_DATA_NAME(name)))

/** @brief Define a FlashString containing data from an external file
 *  @param name Name for the FlashString object
 *  @param file Path to the file to be included. This should be an absolute path.
 *  @note This provides a more efficient way to read constant (read-only) file data.
 *  The file content is bound into firmware image at link time.
 *  @note The FlashString object must be referenced or the linker won't emit it.
 *  @note Use PROJECT_DIR to locate files in your project's source tree. For example:
 *  		IMPORT_FSTR(myFlashString, PROJECT_DIR "/files/my_flash_file.txt");
 *  Use COMPONENT_PATH within a component.
 */
#define IMPORT_FSTR(name, file)                                                                                        \
	IMPORT_FSTR_DATA(name, file)                                                                                       \
	extern const FlashString name;

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

/**
 * @brief describes a counted string stored in flash memory
 */
class FlashString
{
public:
	// Prevent instantiation or copying
	FlashString() = delete;
	FlashString(const FlashString&) = delete;

	static const FlashString& empty()
	{
		return *FSTR_PTR(&zero);
	}

	uint32_t length() const
	{
		return flashLength;
	}

	uint32_t size() const
	{
		return ALIGNUP(flashLength + 1);
	}

	flash_string_t data() const
	{
		return FPSTR(flashData);
	}

	/**
	 * @brief Read contents of a FlashString into RAM
	 * @param offset Zero-based offset from start of flash data to start reading
	 * @param buffer Where to store data
	 * @bytesToRead How many bytes to read (e.g. size of buffer)
	 * @readCache true to read data via CPU data cache, false to bypass and read flash directly
	 * @retval size_t Number of bytes actually read, may be less than bufSize
	 * @note PROGMEM data is accessed via the CPU data cache, so to avoid degrading performance
	 * you can use this method to read data directly from flash memory.
	 * This is appropriate for infrequently accessed data, especially if it is large.
	 * For example, if storing content using `IMPORT_FSTR` instead of SPIFFS then it
	 * is generally better to avoid contaminating the cache.
	 * @see See also `FlashMemoryDataStream` class.
	 */
	size_t read(size_t offset, void* buffer, size_t bytesToRead, bool readCache) const;

	/** @brief Check for equality with a C-string
	 *  @param cstr
	 *  @param len Length of cstr (optional)
	 *  @retval bool true if strings are identical
	 *  @note loads string into a stack buffer for the comparison, no heap required
	 */
	bool isEqual(const char* cstr, size_t len = 0) const;

	/** @brief Check for equality with another FlashString
	 *  @param str
	 *  @retval bool true if strings are identical
	 */
	bool isEqual(const FlashString& str) const;

	bool isEqual(const String& str) const
	{
		return str.equals(*this);
	}

	operator String() const
	{
		return String(data(), length());
	}

	bool operator==(const char* str) const
	{
		return isEqual(str);
	}

	bool operator==(const FlashString& str) const
	{
		return isEqual(str);
	}

	bool operator==(const String& str) const
	{
		return isEqual(str);
	}

	bool operator!=(const char* str) const
	{
		return !isEqual(str);
	}

	bool operator!=(const FlashString& str) const
	{
		return !isEqual(str);
	}

	bool operator!=(const String& str) const
	{
		return !isEqual(str);
	}

private:
	uint32_t flashLength;	 ///< Number of bytes/characters in data
	char flashData[0x400000]; ///< Arbitrary large size to stop compiler complaining about array bounds
	static constexpr uint32_t zero = 0;
};

/**
 * @brief Declare a global table of FlashStrings
 */
#define DECLARE_FSTR_TABLE(name) extern "C" const FlashStringTable name;

#define ARGSIZE(...) (sizeof((const void*[]){__VA_ARGS__}) / sizeof(void*))

#define DEFINE_FSTR_TABLE_DATA(name, ...)                                                                                   \
	const struct { \
		FlashStringTable table;\
		const FlashString* data[ARGSIZE(__VA_ARGS__)];\
	} name PROGMEM = { {ARGSIZE(__VA_ARGS__)}, {__VA_ARGS__}};

#define DEFINE_FSTR_TABLE(name, ...)                                                                                   \
		extern "C" DEFINE_FSTR_TABLE_DATA(FSTR_DATA_NAME(name), __VA_ARGS__);\
		DEFINE_FSTR_TABLE_ALIAS(name, FSTR_DATA_NAME(name));

#define DEFINE_FSTR_TABLE_LOCAL(name, ...)                                                                                   \
		static DEFINE_FSTR_TABLE_DATA(FSTR_DATA_NAME(name), __VA_ARGS__);\
		const FlashStringTable& name = FSTR_DATA_NAME(name).table;

#define DEFINE_FSTR_TABLE_REF(name, data_name) const FlashStringTable& name = *FSTR_TABLE_PTR(&data_name);

#define FSTR_TABLE_PTR(data_ptr) reinterpret_cast<const FlashStringTable*>(data_ptr)

#define DEFINE_FSTR_TABLE_ALIAS(name, data_name)                                                                             \
	extern "C" const FlashStringTable __attribute__((alias(MACROQUOTE(data_name)))) name;

/**
 * @brief Class to access a table of flash strings
 */
class FlashStringTable
{
public:
	FlashStringTable() = delete;
	FlashStringTable(const FlashStringTable&) = delete;

	const FlashString& operator[](unsigned index) const
	{
		if(index < tableLength) {
			auto p = reinterpret_cast<const FlashString*const*>(&tableLength + 1);
			p += index;
			return **p;
		} else {
			return FlashString::empty();
		}
	}

	unsigned length() const
	{
		return tableLength;
	}

	uint32_t tableLength;
//	const FlashString* data[];
};

/**
 * @brief Declare a FlashStringMap
 * @param name name of the map
 * @note Use `DEFINE_FSTR_MAP` to instantiate the global object
 */
#define DECLARE_FSTR_MAP(name) extern const FlashStringMap name;

/**
 * @brief describes a FlashString mapping key => data
 */
struct FlashStringPair {
	const FlashString* key;
	const FlashString* content;
};

class FlashStringMapEntry
{
	typedef void (FlashStringMapEntry::*IfHelperType)() const;
	void IfHelper() const
	{
	}

public:
	FlashStringMapEntry() = default;

	FlashStringMapEntry(const FlashStringPair* pair) : value(pair)
	{
	}

	operator IfHelperType() const
	{
		return value ? &FlashStringMapEntry::IfHelper : 0;
	}

	const FlashString& key() const
	{
		if(value == nullptr) {
			return FlashString::empty();
		} else {
			return *value->key;
		}
	}

	const FlashString& content() const
	{
		if(value == nullptr) {
			return FlashString::empty();
		} else {
			return *value->content;
		}
	}

	operator const FlashString&() const
	{
		return content();
	}

private:
	const FlashStringPair* value;
};

/**
 * @brief Define a map of `FlashString => FlashString` and global FlashStringMap object
 * @param name name of the map
 */
#define DEFINE_FSTR_MAP(name, ...)                                                                                     \
	static const FlashStringPair FSTR_DATA_NAME(name)[] PROGMEM = {__VA_ARGS__};                                       \
	const FlashStringMap name(FSTR_DATA_NAME(name), ARRAY_SIZE(FSTR_DATA_NAME(name)));

/**
 * @brief Define a table of FlashStrings (local scope)
 * @param name name of the table
 */
#define DEFINE_FSTR_MAP_LOCAL(name, ...)                                                                               \
	static const FlashStringPair FSTR_DATA_NAME(name)[] PROGMEM = {__VA_ARGS__};                                       \
	static const FlashStringMap name(FSTR_DATA_NAME(name), ARRAY_SIZE(FSTR_DATA_NAME(name)));

/**
 * @brief Class to access a flash string map
 */
class FlashStringMap
{
public:
	FlashStringMap(const FlashStringPair data[], unsigned count) : data(data), count_(count)
	{
	}

	FlashStringMapEntry valueAt(unsigned index) const
	{
		return (index < count_) ? &data[index] : nullptr;
	}

	template <typename TKey> int indexOf(const TKey& key) const;

	template <typename TKey> FlashStringMapEntry operator[](const TKey& key) const
	{
		int i = indexOf(key);
		return (i < 0) ? nullptr : &data[i];
	}

	unsigned count() const
	{
		return count_;
	}

private:
	const FlashStringPair* data;
	unsigned count_;
};

template <typename TKey> int FlashStringMap::indexOf(const TKey& key) const
{
	for(unsigned i = 0; i < count_; ++i) {
		if(*data[i].key == key) {
			return i;
		}
	}

	return -1;
}
