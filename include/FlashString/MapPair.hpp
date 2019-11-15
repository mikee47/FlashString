/**
 * MapPair.hpp - Defines the MapPair class template
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

#include "String.hpp"

namespace FSTR
{
/**
 * @brief describes a pair mapping key => data for a specified key type
 */
template <typename KeyType, class ContentType> class MapPair
{
	typedef typename std::conditional<std::is_same<KeyType, String>::value, const KeyType*, const KeyType>::type
		KeyStoreType;

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
		return MapPair{KeyStoreType(0), nullptr};
	}

	/**
	 * @brief Get the key (non-class key types)
	 */
	template <typename T = KeyType> typename std::enable_if<!std::is_class<T>::value, KeyType>::type key() const
	{
		// Ensure access is aligned for 1/2 byte keys
		volatile auto pair = *this;
		return pair.key_;
	}

	/**
	 * @brief Get the key (String key type)
	 */
	template <typename T = KeyType>
	typename std::enable_if<std::is_same<T, String>::value, const KeyType&>::type key() const
	{
		auto k = key_;
		if(k == nullptr) {
			return KeyType::empty();
		} else {
			return *k;
		}
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

	/* WString support */

	explicit operator WString() const;

	/* Private member data */

	KeyStoreType key_;
	const ContentType* content_;
};

} // namespace FSTR
