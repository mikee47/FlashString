/**
 * Map.hpp - Defines the Map class template and associated macros
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

#include "Object.hpp"
#include "MapPair.hpp"
#include "MapPrinter.hpp"
#include "ObjectIterator.hpp"

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
	static DEFINE_FSTR_MAP_DATA(FSTR_DATA_NAME(name), KeyType, ContentType, __VA_ARGS__);                              \
	DEFINE_FSTR_REF_NAMED(name, DECL((FSTR::Map<KeyType, ContentType>)));

#define DEFINE_FSTR_MAP_LOCAL(name, KeyType, ContentType, ...)                                                         \
	static DEFINE_FSTR_MAP_DATA(FSTR_DATA_NAME(name), KeyType, ContentType, __VA_ARGS__);                              \
	static constexpr DEFINE_FSTR_REF_NAMED(name, DECL((FSTR::Map<KeyType, ContentType>)));

#define DEFINE_FSTR_MAP_SIZED(name, KeyType, ContentType, size, ...)                                                   \
	static DEFINE_FSTR_MAP_DATA_SIZED(FSTR_DATA_NAME(name), KeyType, ContentType, size, __VA_ARGS__);                  \
	DEFINE_FSTR_REF_NAMED(name, DECL((FSTR::Map<KeyType, ContentType>)));

#define DEFINE_FSTR_MAP_SIZED_LOCAL(name, KeyType, ContentType, size, ...)                                             \
	static DEFINE_FSTR_MAP_DATA_SIZED(FSTR_DATA_NAME(name), KeyType, ContentType, size, __VA_ARGS__);                  \
	static constexpr DEFINE_FSTR_REF_NAMED(name, DECL((FSTR::Map<KeyType, ContentType>)));

/**
 * @brief Define a structure containing map data
 * @param name name of the map structure
 */
#define DEFINE_FSTR_MAP_DATA(name, KeyType, ContentType, ...)                                                          \
	DEFINE_FSTR_MAP_DATA_SIZED(name, KeyType, ContentType,                                                             \
							   (sizeof((const FSTR::MapPair<KeyType, ContentType>[]){__VA_ARGS__}) /                   \
								sizeof(FSTR::MapPair<KeyType, ContentType>)),                                          \
							   __VA_ARGS__)

/**
 * @brief Use in situations where the array size cannot be automatically calculated,
 * such as when combined with inline Strings via FS()
 */
#define DEFINE_FSTR_MAP_DATA_SIZED(name, KeyType, ContentType, size, ...)                                              \
	constexpr const struct {                                                                                           \
		FSTR::ObjectBase object;                                                                                       \
		FSTR::MapPair<KeyType, ContentType> data[size];                                                                \
	} FSTR_PACKED name PROGMEM = {{sizeof(name.data)}, {__VA_ARGS__}};                                                 \
	FSTR_CHECK_STRUCT(name);

namespace FSTR
{
/**
 * @brief Class template to access an associative map
 */
template <typename KeyType, class ContentType, class Pair = MapPair<KeyType, ContentType>>
class Map : public Object<Map<KeyType, ContentType>, Pair>
{
public:
	/**
	 * @brief Get a map entry by index, if it exists
	 * @note Result validity can be checked using if()
	 */
	const Pair valueAt(unsigned index) const
	{
		if(index >= this->length()) {
			return Pair::empty();
		}

		static_assert(offsetof(Pair, content_) == sizeof(uint32_t), "Misaligned MapPair");

		auto ptr = this->data() + index;
		return Pair{readValue(&ptr->key_), readValue(&ptr->content_)};
	}

	/**
	 * @brief Lookup an integral key and return the index
	 * @retval int If key isn't found, return -1
	 */
	template <typename TRefKey, typename T = KeyType>
	typename std::enable_if<!std::is_class<T>::value, int>::type indexOf(const TRefKey& key) const
	{
		auto p = this->data();
		auto len = this->length();
		for(unsigned i = 0; i < len; ++i, ++p) {
			if(p->key() == key) {
				return i;
			}
		}

		return -1;
	}

	/**
	 * @brief Lookup a String key and return the index
	 * @retval int If key isn't found, return -1
	 */
	template <typename TRefKey, typename T = KeyType>
	typename std::enable_if<std::is_same<T, String>::value, int>::type indexOf(const TRefKey& key,
																			   bool ignoreCase = true) const
	{
		auto p = this->data();
		auto len = this->length();
		for(unsigned i = 0; i < len; ++i, ++p) {
			if(ignoreCase) {
				if(p->key().equalsIgnoreCase(key)) {
					return i;
				}
			} else if(p->key() == key) {
				return i;
			}
		}

		return -1;
	}

	/**
	 * @brief Lookup a key and return the entry, if found
	 * @note Result validity can be checked using if()
	 */
	template <typename TRefKey> const Pair operator[](const TRefKey& key) const
	{
		return valueAt(indexOf(key));
	}

	/* Arduino Print support */

	/**
	 * @brief Returns a printer object for this array
	 * @note ElementType must be supported by Print
	 */
	MapPrinter<Map> printer() const
	{
		return MapPrinter<Map>(*this);
	}

	size_t printTo(Print& p) const
	{
		return printer().printTo(p);
	}
};

} // namespace FSTR
