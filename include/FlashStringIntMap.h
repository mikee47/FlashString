/**
 * FlashStringIntMap.h - Associative map of integral type => FlashString
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
 * @brief Declare a FlashStringIntMap
 * @tparam TKey Integral type to use for key
 * @param name name of the map
 * @note Use `DEFINE_FSTR_INTMAP` to instantiate the global object
 */
#define DECLARE_FSTR_INTMAP(name, TKey) extern const FlashStringIntMap<TKey>& name;

/**
 * @brief Define a FlashString IntMap with reference
 */
#define DEFINE_FSTR_INTMAP(name, TKey, ...)                                                                            \
	DEFINE_FSTR_INTMAP_DATA(FSTR_DATA_NAME(name), TKey, __VA_ARGS__);                                                  \
	const FlashStringIntMap<TKey>& name PROGMEM = FSTR_DATA_NAME(name).map;

#define DEFINE_FSTR_INTMAP_LOCAL(name, TKey, ...)                                                                      \
	DEFINE_FSTR_INTMAP_DATA_LOCAL(FSTR_DATA_NAME(name), TKey, __VA_ARGS__);                                            \
	static const FlashStringIntMap<TKey>& name PROGMEM = FSTR_DATA_NAME(name).map;

/**
 * @brief Cast a pointer to FlashStringIntMap*
 */
#define FSTR_INTMAP_PTR(data_ptr, TKey) reinterpret_cast<const FlashStringIntMap<TKey>*>(data_ptr)

/**
 * @brief Define a FlashStringIntMap& reference using a cast
 */
#define DEFINE_FSTR_INTMAP_REF(name, TKey, data_name) const FlashStringIntMap<TKey>& name = *FSTR_MAP_PTR(&data_name);

/**
 * @brief Define a structure containing map data
 * @param name name of the map structure
 */
#define FSTR_INTMAP_ARGSIZE(TKey, ...)                                                                                 \
	(sizeof((const FlashStringIntPair<TKey>[]){__VA_ARGS__}) / sizeof(FlashStringIntPair<TKey>))
#define DEFINE_FSTR_INTMAP_DATA(name, TKey, ...)                                                                       \
	const struct {                                                                                                     \
		FlashStringIntMap<TKey> map;                                                                                   \
		const FlashStringIntPair<TKey> data[FSTR_INTMAP_ARGSIZE(TKey, __VA_ARGS__)];                                   \
	} name PROGMEM = {{FSTR_INTMAP_ARGSIZE(TKey, __VA_ARGS__)}, {__VA_ARGS__}};
#define DEFINE_FSTR_INTMAP_DATA_LOCAL(name, TKey, ...) static DEFINE_FSTR_INTMAP_DATA(name, TKey, __VA_ARGS__)

/**
 * @brief describes a FlashString mapping key => data for a specified key type
 */
template <typename TKey> struct FlashStringIntPair {
	typedef void (FlashStringIntPair::*IfHelperType)() const;
	void IfHelper() const
	{
	}

	/**
	 * @brief Provides bool() operator to determine if Pair is valid
	 */
	operator IfHelperType() const
	{
		return content_ ? &FlashStringIntPair::IfHelper : 0;
	}

	/**
	 * @brief Get an empty Pair object, identifies as invalid when lookup fails
	 */
	static const FlashStringIntPair empty()
	{
		return FlashStringIntPair({TKey(0), nullptr});
	}

	/**
	 * @brief Accessor to get a copy of the key
	 */
	TKey key() const
	{
		// Ensure access is aligned for 1/2 byte keys
		volatile auto pair = *this;
		return pair.key_;
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

	operator const FlashString&() const
	{
		return content();
	}

	operator String() const
	{
		return content();
	}

	const TKey key_;
	const FlashString* content_;
};

/**
 * @brief Class to access a flash string map
 */
template <typename TKey> struct FlashStringIntMap {
	using Pair = FlashStringIntPair<TKey>;

	/**
	 * @brief Get a map entry by index, if it exists
	 * @note Result validity can be checked using if()
	 */
	const Pair valueAt(unsigned index) const
	{
		if(index >= mapLength) {
			return Pair::empty();
		}

		auto p = reinterpret_cast<const Pair*>(&mapLength + 1);
		p += index;
		return *p;
	}

	/**
	 * @brief Lookup a key and return the index, if found
	 * @retval int If key isn't found, return -1
	 * @note Comparison is case-sensitive
	 */
	template <typename TKeyRef> int indexOf(const TKeyRef& key) const;

	/**
	 * @brief Lookup a key and return the entry, if found
	 * @note Result validity can be checked using if()
	 */
	template <typename TKeyRef> const Pair operator[](const TKeyRef& key) const
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
	// Pair values[];
};

template <typename TKey> template <typename TKeyRef> int FlashStringIntMap<TKey>::indexOf(const TKeyRef& key) const
{
	auto p = reinterpret_cast<const Pair*>(&mapLength + 1);
	for(unsigned i = 0; i < mapLength; ++i, ++p) {
		if(IS_ALIGNED(sizeof(TKey))) {
			if(p->key_ == key) {
				return i;
			}
		} else {
			// Ensure access is aligned for 1/2 byte keys
			volatile auto pair = *p;
			if(pair.key_ == key) {
				return i;
			}
		}
	}

	return -1;
}
