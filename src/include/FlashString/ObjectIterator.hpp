/**
 * ObjectIterator.hpp - STL iterator support
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

#include <iterator>

namespace FSTR
{
template <class ObjectType, typename ElementType>
class ObjectIterator : public std::iterator<std::random_access_iterator_tag, ElementType>
{
public:
	ObjectIterator() = default;
	ObjectIterator(const ObjectIterator&) = default;

	ObjectIterator(const ObjectType& object, unsigned index) : object(object), index(index)
	{
	}

	ObjectIterator& operator++()
	{
		++index;
		return *this;
	}

	ObjectIterator operator++(int)
	{
		ObjectIterator tmp(*this);
		++index;
		return tmp;
	}

	ObjectIterator operator+=(size_t distance)
	{
		ObjectIterator tmp(*this);
		index += distance;
		return tmp;
	}

	bool operator==(const ObjectIterator& rhs) const
	{
		return index == rhs.index;
	}

	bool operator!=(const ObjectIterator& rhs) const
	{
		return index != rhs.index;
	}

	/**
	 * @brief Accessor returns a copy for non-pointer-type elements
	 */
	template <typename T = ElementType>
	typename std::enable_if<!std::is_pointer<T>::value, const ElementType>::type operator*() const
	{
		return object.valueAt(index);
	}

	/**
	 * @brief Accessor returns a reference for pointer-type elements
	 */
	template <typename T = ElementType>
	typename std::enable_if<std::is_pointer<T>::value, const typename std::remove_pointer<ElementType>::type&>::type
	operator*() const
	{
		return object.valueAt(index);
	}

private:
	const ObjectType& object;
	unsigned index = 0;
};

} // namespace FSTR
