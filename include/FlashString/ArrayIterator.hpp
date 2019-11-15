/**
 * ArrayIterator.hpp - STL iterator to handle array of integral elements or structs
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
 * @author: 2018 - Mikee47 <mike@sillyhouse.net>
 *
 ****/

#pragma once

#include <iterator>

namespace FSTR
{
template <class ArrayType, typename ElementType>
class ArrayIterator : public std::iterator<std::random_access_iterator_tag, ElementType>
{
public:
	ArrayIterator() = default;
	ArrayIterator(const ArrayIterator&) = default;

	ArrayIterator(const ArrayType& array, unsigned index) : array(array), index(index)
	{
	}

	ArrayIterator& operator++()
	{
		++index;
		return *this;
	}

	ArrayIterator operator++(int)
	{
		ArrayIterator tmp(*this);
		++index;
		return tmp;
	}

	ArrayIterator operator+=(size_t distance)
	{
		ArrayIterator tmp(*this);
		index += distance;
		return tmp;
	}

	bool operator==(const ArrayIterator& rhs) const
	{
		return index == rhs.index;
	}

	bool operator!=(const ArrayIterator& rhs) const
	{
		return index != rhs.index;
	}

	const ElementType operator*() const
	{
		return array.valueAt(index);
	}

private:
	const ArrayType& array;
	unsigned index = 0;
};

} // namespace FSTR
