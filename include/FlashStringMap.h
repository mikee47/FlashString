/****
 * Sming Framework Project - Open Source framework for high efficiency native ESP8266 development.
 * Created 2015 by Skurydin Alexey
 * http://github.com/SmingHub/Sming
 * All files of the Sming Core are provided under the LGPL v3 license.
 *
 * FlashStringMap.h - Defines the FlashStringMap class and associated macros
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

#define DEFINE_FSTR_MAP(name, ...)                                                                                     \
	DEFINE_FSTR_MAP_DATA(FSTR_DATA_NAME(name), __VA_ARGS__);                                                           \
	const FlashStringMap& name = FSTR_DATA_NAME(name).map;

#define DEFINE_FSTR_MAP_LOCAL(name, ...)                                                                               \
	DEFINE_FSTR_MAP_DATA_LOCAL(FSTR_DATA_NAME(name), __VA_ARGS__);                                                     \
	static const FlashStringMap& name = FSTR_DATA_NAME(name).map;

#define DEFINE_FSTR_MAP_REF(name, data_name) const FlashStringMap& name = *FSTR_MAP_PTR(&data_name);
#define FSTR_MAP_PTR(data_ptr) reinterpret_cast<const FlashStringMap*>(data_ptr)

/**
 * @brief Define a map of `FlashString => FlashString` and global FlashStringMap object
 * @param name name of the map
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

	operator IfHelperType() const
	{
		return key_ ? &FlashStringPair::IfHelper : 0;
	}

	static const FlashStringPair& empty()
	{
		static constexpr FlashStringPair nullPair PROGMEM = {nullptr, nullptr};
		return nullPair;
	}

	const FlashString& key() const
	{
		if(key_ == nullptr) {
			return FlashString::empty();
		} else {
			return *key_;
		}
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

	const FlashString* key_;
	const FlashString* content_;
};

/**
 * @brief Class to access a flash string map
 */
struct FlashStringMap {
	const FlashStringPair& valueAt(unsigned index) const
	{
		if(index >= mapLength) {
			return FlashStringPair::empty();
		}

		auto p = reinterpret_cast<const FlashStringPair*>(&mapLength + 1);
		p += index;
		return *p;
	}

	template <typename TKey> int indexOf(const TKey& key) const;

	template <typename TKey> const FlashStringPair& operator[](const TKey& key) const
	{
		return valueAt(indexOf(key));
	}

	unsigned length() const
	{
		return mapLength;
	}

	uint32_t mapLength;
	// FlashStringPair values[];
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
