/**
 * FlashStringMap.h - Defines the FlashStringMap class and associated macros
 *
 * Copyright 2019 mikee47 <mike@sillyhouse.net>
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
 * @author: Nov 2019 - Mikee47 <mike@sillyhouse.net>
 *
 ****/

#pragma once

#include "FlashString.h"

/**
 * @brief Declare a FlashStringMap
 * @param name name of the map
 * @note Use `DEFINE_FSTR_MAP` to instantiate the global object
 */
#define DECLARE_FSTR_MAP(name) extern const FlashStringMap& name;

/**
 * @brief Define a FlashString Map with reference
 */
#define DEFINE_FSTR_MAP(name, ...)                                                                                     \
	DEFINE_FSTR_MAP_DATA(FSTR_DATA_NAME(name), __VA_ARGS__);                                                           \
	const FlashStringMap& name PROGMEM = FSTR_DATA_NAME(name).map;

#define DEFINE_FSTR_MAP_LOCAL(name, ...)                                                                               \
	DEFINE_FSTR_MAP_DATA_LOCAL(FSTR_DATA_NAME(name), __VA_ARGS__);                                                     \
	static const FlashStringMap& name PROGMEM = FSTR_DATA_NAME(name).map;

/**
 * @brief Cast a pointer to FlashStringMap*
 */
#define FSTR_MAP_PTR(data_ptr) reinterpret_cast<const FlashStringMap*>(data_ptr)

/**
 * @brief Define a FlashStringMap& reference using a cast
 */
#define DEFINE_FSTR_MAP_REF(name, data_name) const FlashStringMap& name = *FSTR_MAP_PTR(&data_name);

/**
 * @brief Define a structure containing map data
 * @param name name of the map structure
 */
#define FSTR_MAP_ARGSIZE(...) (sizeof((const FlashStringPair[]){__VA_ARGS__}) / sizeof(FlashStringPair))
#define DEFINE_FSTR_MAP_DATA(name, ...)                                                                                \
	const struct {                                                                                                     \
		FlashStringMap map;                                                                                            \
		const FlashStringPair data[FSTR_MAP_ARGSIZE(__VA_ARGS__)];                                                     \
	} name PROGMEM = {{FSTR_MAP_ARGSIZE(__VA_ARGS__)}, {__VA_ARGS__}};
#define DEFINE_FSTR_MAP_DATA_LOCAL(name, ...) static DEFINE_FSTR_MAP_DATA(name, __VA_ARGS__)

/**
 * @brief describes a FlashString mapping key => data
 */
struct FlashStringPair {
	typedef void (FlashStringPair::*IfHelperType)() const;
	void IfHelper() const
	{
	}

	/**
	 * @brief Provides bool() operator to determine if Pair is valid
	 */
	operator IfHelperType() const
	{
		return key_ ? &FlashStringPair::IfHelper : 0;
	}

	/**
	 * @brief Get an empty Pair object, identifies as invalid when lookup fails
	 */
	static const FlashStringPair empty()
	{
		return FlashStringPair({nullptr, nullptr});
	}

	/**
	 * @brief Accessor to get a reference to the key
	 */
	const FlashString& key() const
	{
		if(key_ == nullptr) {
			return FlashString::empty();
		} else {
			return *key_;
		}
	}

	/**
	 * @brief Accessor to get a reference to the content
	 */
	const FlashString& content() const
	{
		if(content_ == nullptr) {
			return FlashString::empty();
		} else {
			return *content_;
		}
	}

	/**
	 * @brief Implicit FlashString cast for this object gets a reference to the content
	 */
	operator const FlashString&() const
	{
		return content();
	}

	/**
	 * @brief Implicit cast to load content into a String object
	 */
	operator String() const
	{
		return content();
	}

	const FlashString* key_;
	const FlashString* content_;
};

/**
 * @brief Class to access a flash string map
 */
struct FlashStringMap {
	/**
	 * @brief Get a map entry by index, if it exists
	 * @note Result validity can be checked using if()
	 */
	const FlashStringPair valueAt(unsigned index) const
	{
		if(index >= mapLength) {
			return FlashStringPair::empty();
		}

		auto p = reinterpret_cast<const FlashStringPair*>(&mapLength + 1);
		p += index;
		return *p;
	}

	/**
	 * @brief Lookup a key and return the index, if found
	 * @retval int If key isn't found, return -1
	 * @note Comparison is case-sensitive
	 */
	template <typename TKey> int indexOf(const TKey& key) const;

	/**
	 * @brief Lookup a key and return the entry, if found
	 * @note Result validity can be checked using if()
	 */
	template <typename TKey> const FlashStringPair operator[](const TKey& key) const
	{
		return valueAt(indexOf(key));
	}

	/**
	 * @brief Accessor to get the number of entries in the map
	 */
	unsigned length() const
	{
		return mapLength;
	}

	const uint32_t mapLength;
	// const FlashStringPair values[];
};

template <typename TKey> int FlashStringMap::indexOf(const TKey& key) const
{
	auto p = reinterpret_cast<const FlashStringPair*>(&mapLength + 1);
	for(unsigned i = 0; i < mapLength; ++i, ++p) {
		if(*p->key_ == key) {
			return i;
		}
	}

	return -1;
}
