/**
 * MapPair.h - Defines the MapPair class template
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
#include <WString.h>

namespace FSTR
{
/**
 * @brief describes a pair mapping key => data for a specified key type
 */
template <typename KeyType, class ContentType> class MapPair
{
public:
	typedef void (MapPair::*IfHelperType)() const;
	void IfHelper() const
	{
	}

	/**
	 * @brief Provides bool() operator to determine if Pair is valid
	 */
	operator IfHelperType() const
	{
		return content_ ? &MapPair::IfHelper : 0;
	}

	/**
	 * @brief Get an empty Pair object, identifies as invalid when lookup fails
	 */
	static const MapPair empty()
	{
		return MapPair{static_cast<const KeyType>(0), static_cast<const ContentType*>(0)};
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

	/* Arduino String support */

	operator ::String() const
	{
		return ::String(content());
	}

	/* Private member data */

	const KeyType key_;
	const ContentType* content_;
};

template <class ContentType> class MapPair<String*, ContentType>
{
	typedef void (MapPair::*IfHelperType)() const;
	void IfHelper() const
	{
	}

public:
	operator IfHelperType() const
	{
		return content_ ? &MapPair::IfHelper : 0;
	}

	static const MapPair& empty()
	{
		static const MapPair PROGMEM empty_{nullptr, static_cast<const ContentType*>(0)};
		return empty_;
	}

	const String& key() const
	{
		auto k = key_;
		if(k == nullptr) {
			return String::empty();
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

	/* Arduino String support */

	operator ::String() const
	{
		return ::String(content());
	}

	/* Private member data */

	const String* key_;
	const ContentType* content_;
};

} // namespace FSTR
