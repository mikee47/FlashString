/**
 * ObjectIterator.h - STL iterator to handle array of objects (String, Table, Map, Pair or custom)
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
 * @author: 2018 - Mikee47 <mike@sillyhouse.net>
 *
 ****/

#pragma once

#include <iterator>

namespace FSTR
{
template <typename ObjectType>
class ObjectIterator : public std::iterator<std::random_access_iterator_tag, const ObjectType>
{
public:
	ObjectIterator() = default;
	ObjectIterator(const ObjectIterator&) = default;

	ObjectIterator(const ObjectType* const* head, unsigned count, unsigned index)
		: head(head), count(count), index(index)
	{
	}

	ObjectIterator& operator++()
	{
		if(index < count) {
			++index;
		}
		return *this;
	}

	ObjectIterator operator++(int)
	{
		ObjectIterator tmp(*this);
		if(index < count) {
			++index;
		}
		return tmp;
	}

	ObjectIterator operator+=(size_t distanc)
	{
		ObjectIterator tmp(*this);
		if(index < count) {
			++index;
		}
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

	const ObjectType& operator*() const
	{
		return (index < count) ? *head[index] : ObjectType::empty();
	}

private:
	const ObjectType* const* head = nullptr;
	unsigned count = 0;
	unsigned index = 0;
};

} // namespace FSTR
