/****
 * Sming Framework Project - Open Source framework for high efficiency native ESP8266 development.
 * Created 2015 by Skurydin Alexey
 * http://github.com/SmingHub/Sming
 * All files of the Sming Core are provided under the LGPL v3 license.
 *
 * FlashStringIntMap.h - Associative map of integral type => FlashString
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

#define DEFINE_FSTR_INTMAP(name, TKey, ...)                                                                            \
	DEFINE_FSTR_INTMAP_DATA(FSTR_DATA_NAME(name), TKey, __VA_ARGS__);                                                  \
	const FlashStringIntMap<TKey>& name PROGMEM = FSTR_DATA_NAME(name).map;

#define DEFINE_FSTR_INTMAP_LOCAL(name, TKey, ...)                                                                      \
	DEFINE_FSTR_INTMAP_DATA_LOCAL(FSTR_DATA_NAME(name), TKey, __VA_ARGS__);                                            \
	static const FlashStringIntMap<TKey>& name PROGMEM = FSTR_DATA_NAME(name).map;

#define DEFINE_FSTR_INTMAP_REF(name, TKey, data_name) const FlashStringIntMap<TKey>& name = *FSTR_MAP_PTR(&data_name);
#define FSTR_INTMAP_PTR(data_ptr, TKey) reinterpret_cast<const FlashStringIntMap<TKey>*>(data_ptr)

/**
 * @brief Define a map of `TKey => FlashString` and global FlashStringMap object
 * @param name name of the map
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
 * @brief describes a FlashString mapping key => data
 */
template <typename TKey> struct FlashStringIntPair {
	typedef void (FlashStringIntPair::*IfHelperType)() const;
	void IfHelper() const
	{
	}

	operator IfHelperType() const
	{
		return content_ ? &FlashStringIntPair::IfHelper : 0;
	}

	static const FlashStringIntPair empty()
	{
		return FlashStringIntPair({TKey(0), nullptr});
	}

	TKey key() const
	{
		// Ensure access is aligned for 1/2 byte keys
		volatile auto pair = *this;
		return pair.key_;
	}

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

	const Pair valueAt(unsigned index) const
	{
		if(index >= mapLength) {
			return Pair::empty();
		}

		auto p = reinterpret_cast<const Pair*>(&mapLength + 1);
		p += index;
		return *p;
	}

	template <typename TKeyRef> int indexOf(const TKeyRef& key) const;

	template <typename TKeyRef> const Pair operator[](const TKeyRef& key) const
	{
		return valueAt(indexOf(key));
	}

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
