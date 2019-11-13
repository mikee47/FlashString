/**
 * FlashStringMap.h - Associative map of integral type => FlashString
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
 * @tparam KeyType Integral type to use for key
 * @param name name of the map
 * @note Use `DEFINE_FSTR_MAP` to instantiate the global object
 */
#define DECLARE_FSTR_MAP(name, KeyType, ContentType) extern const FlashStringMap<KeyType, ContentType>& name;

/**
 * @brief Define a FlashString Map with reference
 */
#define DEFINE_FSTR_MAP(name, KeyType, ContentType, ...)                                                               \
	DEFINE_FSTR_MAP_DATA(FSTR_DATA_NAME(name), KeyType, ContentType, __VA_ARGS__);                                     \
	const FlashStringMap<KeyType, ContentType>& name PROGMEM = FSTR_DATA_NAME(name).map;

#define DEFINE_FSTR_MAP_LOCAL(name, KeyType, ContentType, ...)                                                         \
	DEFINE_FSTR_MAP_DATA_LOCAL(FSTR_DATA_NAME(name), KeyType, ContentType, __VA_ARGS__);                               \
	static const FlashStringMap<KeyType, ContentType>& name PROGMEM = FSTR_DATA_NAME(name).map;

/**
 * @brief Cast a pointer to FlashStringMap*
 */
#define FSTR_MAP_PTR(data_ptr, KeyType, ContentType)                                                                   \
	reinterpret_cast<const FlashStringMap<KeyType, ContentType>*>(data_ptr)

/**
 * @brief Define a FlashStringMap& reference using a cast
 */
#define DEFINE_FSTR_MAP_REF(name, KeyType, ContentType, data_name)                                                     \
	const FlashStringMap<KeyType, ContentType>& name = *FSTR_MAP_PTR(&data_name);

/**
 * @brief Define a structure containing map data
 * @param name name of the map structure
 */
#define FSTR_MAP_ARGSIZE(KeyType, ContentType, ...)                                                                    \
	(sizeof((const FlashStringIntPair<KeyType, ContentType>[]){__VA_ARGS__}) /                                         \
	 sizeof(FlashStringIntPair<KeyType, ContentType>))
#define DEFINE_FSTR_MAP_DATA(name, KeyType, ContentType, ...)                                                          \
	constexpr struct {                                                                                                 \
		FlashStringMap<KeyType, ContentType> map;                                                                      \
		FlashStringIntPair<KeyType, ContentType> data[FSTR_MAP_ARGSIZE(KeyType, ContentType, __VA_ARGS__)];            \
	} name PROGMEM = {{FSTR_MAP_ARGSIZE(KeyType, ContentType, __VA_ARGS__)}, {__VA_ARGS__}};
#define DEFINE_FSTR_MAP_DATA_LOCAL(name, KeyType, ContentType, ...)                                                    \
	static DEFINE_FSTR_MAP_DATA(name, KeyType, ContentType, __VA_ARGS__)

/**
 * @brief describes a FlashString mapping key => data for a specified key type
 */
template <typename KeyType, class ContentType> struct FlashStringIntPair {
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
		return FlashStringIntPair{static_cast<const KeyType>(0), static_cast<const ContentType*>(0)};
	}

	KeyType key() const
	{
		// Ensure access is aligned for 1/2 byte keys
		volatile auto pair = *this;
		return pair.key_;
	}

	/**
	 * @brief Accessor to get a reference to the content
	 */
	const ContentType& content() const
	{
		if(content_ == nullptr) {
			return ContentType::empty();
		} else {
			return *content_;
		}
	}

	operator const ContentType&() const
	{
		return content();
	}

	operator String() const
	{
		return String(content());
	}

	const KeyType key_;
	const ContentType* content_;
};

template <class ContentType> struct FlashStringIntPair<FlashString*, ContentType> {
	typedef void (FlashStringIntPair::*IfHelperType)() const;
	void IfHelper() const
	{
	}

	operator IfHelperType() const
	{
		return content_ ? &FlashStringIntPair::IfHelper : 0;
	}

	static const FlashStringIntPair& empty()
	{
		static FlashStringIntPair empty_{nullptr, static_cast<const ContentType*>(0)};
		return empty_;
	}

	const FlashString& key() const
	{
		auto k = key_;
		if(k == nullptr) {
			return FlashString::empty();
		} else {
			return *k;
		}
	}

	const ContentType& content() const
	{
		if(content_ == nullptr) {
			return ContentType::empty();
		} else {
			return *content_;
		}
	}

	operator const ContentType&() const
	{
		return content();
	}

	operator String() const
	{
		return String(content());
	}

	const FlashString* key_;
	const ContentType* content_;
};

/**
 * @brief Class to access a flash string map
 */
template <typename KeyType, class ContentType> struct FlashStringMap {
	using Pair = const FlashStringIntPair<KeyType, ContentType>;

	static const FlashStringMap& empty()
	{
		static FlashStringMap empty_{0};
		return empty_;
	}

	/**
	 * @brief Get a map entry by index, if it exists
	 * @note Result validity can be checked using if()
	 */
	Pair valueAt(unsigned index) const
	{
		if(index >= mapLength) {
			return Pair::empty();
		}

		auto p = reinterpret_cast<Pair*>(&mapLength + 1);
		p += index;
		return *p;
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

	operator String() const
	{
		return nullptr;
	}

	const uint32_t mapLength;
	// Pair values[];
};

template <typename KeyType, class ContentType>
template <typename TRefKey>
int FlashStringMap<KeyType, ContentType>::indexOf(const TRefKey& key) const
{
	auto p = reinterpret_cast<Pair*>(&mapLength + 1);
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

template <class ContentType> struct FlashStringMap<FlashString*, ContentType> {
	using Pair = const FlashStringIntPair<FlashString*, ContentType>;

	Pair valueAt(unsigned index) const
	{
		if(index >= mapLength) {
			return Pair::empty();
		}

		auto p = reinterpret_cast<Pair*>(&mapLength + 1);
		p += index;
		return *p;
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

	const uint32_t mapLength;
};

template <class ContentType>
template <typename TRefKey>
int FlashStringMap<FlashString*, ContentType>::indexOf(const TRefKey& key) const
{
	auto p = reinterpret_cast<Pair*>(&mapLength + 1);
	for(unsigned i = 0; i < mapLength; ++i, ++p) {
		if(*p->key_ == key) {
			return i;
		}
	}

	return -1;
}
