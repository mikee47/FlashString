/**
 * MapPairIterator.h - STL iterator to handle array of MapPair objects
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
template <class Pair> class MapPairIterator : public std::iterator<std::random_access_iterator_tag, Pair>
{
public:
	MapPairIterator() = default;
	MapPairIterator(const MapPairIterator&) = default;

	MapPairIterator(Pair* head, unsigned count, unsigned index) : head(head), count(count), index(index)
	{
	}

	MapPairIterator& operator++()
	{
		if(index < count) {
			++index;
		}
		return *this;
	}

	MapPairIterator operator++(int)
	{
		MapPairIterator tmp(*this);
		if(index < count) {
			++index;
		}
		return tmp;
	}

	MapPairIterator operator+=(size_t distanc)
	{
		MapPairIterator tmp(*this);
		if(index < count) {
			++index;
		}
		return tmp;
	}

	bool operator==(const MapPairIterator& rhs) const
	{
		return index == rhs.index;
	}

	bool operator!=(const MapPairIterator& rhs) const
	{
		return index != rhs.index;
	}

	Pair operator*() const
	{
		return (index < count) ? head[index] : Pair::empty();
	}

private:
	Pair* head = nullptr;
	unsigned count = 0;
	unsigned index = 0;
};

} // namespace FSTR
