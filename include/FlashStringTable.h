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

#pragma once

#include "FlashString.h"
#include <stringutil.h>

/**
 * @brief Declare a global table of FlashStrings
 */
#define DECLARE_FSTR_TABLE(name) extern const FlashStringTable& name;

#define DEFINE_FSTR_TABLE(name, ...)                                                                                   \
	DEFINE_FSTR_TABLE_DATA(FSTR_DATA_NAME(name), __VA_ARGS__);                                                         \
	const FlashStringTable& name PROGMEM = FSTR_DATA_NAME(name).table;

#define DEFINE_FSTR_TABLE_LOCAL(name, ...)                                                                             \
	DEFINE_FSTR_TABLE_DATA_LOCAL(FSTR_DATA_NAME(name), __VA_ARGS__);                                                   \
	static const FlashStringTable& name PROGMEM = FSTR_DATA_NAME(name).table;

#define DEFINE_FSTR_TABLE_REF(name, data_name) const FlashStringTable& name = *FSTR_TABLE_PTR(&data_name);
#define FSTR_TABLE_PTR(data_ptr) reinterpret_cast<const FlashStringTable*>(data_ptr)

#define FSTR_TABLE_ARGSIZE(...) (sizeof((const void* []){__VA_ARGS__}) / sizeof(void*))
#define DEFINE_FSTR_TABLE_DATA(name, ...)                                                                              \
	const struct {                                                                                                     \
		FlashStringTable table;                                                                                        \
		const FlashString* data[FSTR_TABLE_ARGSIZE(__VA_ARGS__)];                                                      \
	} name PROGMEM = {{FSTR_TABLE_ARGSIZE(__VA_ARGS__)}, {__VA_ARGS__}};
#define DEFINE_FSTR_TABLE_DATA_LOCAL(name, ...) static DEFINE_FSTR_TABLE_DATA(name, __VA_ARGS__)

/**
 * @brief Class to access a table of flash strings
 */
struct FlashStringTable {
	const FlashString& operator[](unsigned index) const
	{
		if(index < tableLength) {
			auto p = reinterpret_cast<const FlashString* const*>(&tableLength + 1);
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

	const uint32_t tableLength;
	// const FlashString* entries[];
};
