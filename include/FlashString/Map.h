/**
 * Map.h - Defines the Map class template and associated macros
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
 * @author: Nov 2019 - Mikee47 <mike@sillyhouse.net>
 *
 ****/

#pragma once

#include "String.h"
#include "MapPair.h"
#include "MapPairIterator.h"

/**
 * @brief Declare a Map
 * @tparam KeyType Integral type to use for key
 * @param name name of the map
 * @note Use `DEFINE_FSTR_MAP` to instantiate the global object
 */
#define DECLARE_FSTR_MAP(name, KeyType, ContentType) extern const FSTR::Map<KeyType, ContentType>& name;

/**
 * @brief Define a Map with reference
 */
#define DEFINE_FSTR_MAP(name, KeyType, ContentType, ...)                                                               \
	DEFINE_FSTR_MAP_DATA(FSTR_DATA_NAME(name), KeyType, ContentType, __VA_ARGS__);                                     \
	const FSTR::Map<KeyType, ContentType>& name PROGMEM = FSTR_DATA_NAME(name).map;

#define DEFINE_FSTR_MAP_LOCAL(name, KeyType, ContentType, ...)                                                         \
	DEFINE_FSTR_MAP_DATA_LOCAL(FSTR_DATA_NAME(name), KeyType, ContentType, __VA_ARGS__);                               \
	static const FSTR::Map<KeyType, ContentType>& name PROGMEM = FSTR_DATA_NAME(name).map;

/**
 * @brief Cast a pointer to Map*
 */
#define FSTR_MAP_PTR(data_ptr, KeyType, ContentType) reinterpret_cast<const FSTR::Map<KeyType, ContentType>*>(data_ptr)

/**
 * @brief Define a Map& reference using a cast
 */
#define DEFINE_FSTR_MAP_REF(name, KeyType, ContentType, data_name)                                                     \
	const FSTR::Map<KeyType, ContentType>& name = *FSTR_MAP_PTR(&data_name);

/**
 * @brief Define a structure containing map data
 * @param name name of the map structure
 */
#define FSTR_MAP_ARGSIZE(KeyType, ContentType, ...)                                                                    \
	(sizeof((const FSTR::MapPair<KeyType, ContentType>[]){__VA_ARGS__}) / sizeof(FSTR::MapPair<KeyType, ContentType>))
#define DEFINE_FSTR_MAP_DATA(name, KeyType, ContentType, ...)                                                          \
	constexpr struct {                                                                                                 \
		FSTR::Map<KeyType, ContentType> map;                                                                           \
		FSTR::MapPair<KeyType, ContentType> data[FSTR_MAP_ARGSIZE(KeyType, ContentType, __VA_ARGS__)];                 \
	} name PROGMEM = {{FSTR_MAP_ARGSIZE(KeyType, ContentType, __VA_ARGS__)}, {__VA_ARGS__}};
#define DEFINE_FSTR_MAP_DATA_LOCAL(name, KeyType, ContentType, ...)                                                    \
	static DEFINE_FSTR_MAP_DATA(name, KeyType, ContentType, __VA_ARGS__)

namespace FSTR
{
/**
 * @brief Class template to access an associative map
 */
template <typename KeyType, class ContentType> class Map
{
public:
	using Pair = const MapPair<KeyType, ContentType>;
	using Iterator = MapPairIterator<Pair>;

	Iterator begin() const
	{
		return Iterator(head(), mapLength, 0);
	}

	Iterator end() const
	{
		return Iterator(head(), mapLength, mapLength);
	}

	static const Map& empty()
	{
		static const Map PROGMEM empty_{0};
		return empty_;
	}

	/**
	 * @brief Get a map entry by index, if it exists
	 * @note Result validity can be checked using if()
	 */
	Pair valueAt(unsigned index) const
	{
		return (index < mapLength) ? head()[index] : Pair::empty();
	}

	/**
	 * @brief Lookup a key and return the index, if found
	 * @retval int If key isn't found, return -1
	 * @note Comparison is case-sensitive
	 */
	template <typename TRefKey> int indexOf(const TRefKey& key) const;

	/**
	 * @brief Lookup a key and return the entry, if found
	 * @note Result validity can be checked using if()
	 */
	template <typename TRefKey> Pair operator[](const TRefKey& key) const
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

	const Pair* head() const
	{
		return reinterpret_cast<Pair*>(&mapLength + 1);
	}

	const uint32_t mapLength;
	// const Pair values[];
};

template <typename KeyType, class ContentType>
template <typename TRefKey>
int Map<KeyType, ContentType>::indexOf(const TRefKey& key) const
{
	auto p = head();
	for(unsigned i = 0; i < mapLength; ++i, ++p) {
		if(IS_ALIGNED(sizeof(KeyType))) {
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

/**
 * @brief Specialization for Map with String key
 */
template <class ContentType> class Map<String*, ContentType>
{
public:
	using Pair = const MapPair<String*, ContentType>;
	using Iterator = MapPairIterator<Pair>;

	Iterator begin() const
	{
		return Iterator(head(), mapLength, 0);
	}

	Iterator end() const
	{
		return Iterator(head(), mapLength, mapLength);
	}

	Pair valueAt(unsigned index) const
	{
		return (index < mapLength) ? head()[index] : Pair::empty();
	}

	template <typename TRefKey> int indexOf(const TRefKey& key) const;

	template <typename TRefKey> Pair operator[](const TRefKey& key) const
	{
		return valueAt(indexOf(key));
	}

	unsigned length() const
	{
		return mapLength;
	}

	const Pair* head() const
	{
		return reinterpret_cast<Pair*>(&mapLength + 1);
	}

	/* Private member data */

	const uint32_t mapLength;
	// const Pair values[];
};

template <class ContentType>
template <typename TRefKey>
int Map<String*, ContentType>::indexOf(const TRefKey& key) const
{
	auto p = head();
	for(unsigned i = 0; i < mapLength; ++i, ++p) {
		if(*p->key_ == key) {
			return i;
		}
	}

	return -1;
}

} // namespace FSTR
