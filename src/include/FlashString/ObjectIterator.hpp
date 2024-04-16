/****
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
 * You should have received a copy of the GNU General Public License along with this library.
 * If not, see <https://www.gnu.org/licenses/>.
 *
 * @author: Nov 2019 - Mikee47 <mike@sillyhouse.net>
 *
 ****/

#pragma once

#include <iterator>

namespace FSTR
{
template <class ObjectType, typename ElementType> class ObjectIterator
{
public:
	using iterator_category = std::random_access_iterator_tag;
	using value_type = ElementType;
	using difference_type = std::ptrdiff_t;
	using pointer = typename ObjectType::DataPtrType;
	using reference = ElementType&;

	ObjectIterator() = default;
	ObjectIterator(const ObjectIterator&) = default;
	ObjectIterator(ObjectIterator&&) = default;
	ObjectIterator& operator=(const ObjectIterator&) = default;
	ObjectIterator& operator=(ObjectIterator&&) = default;

	ObjectIterator(const ObjectType& object, unsigned index)
		: data(pointer(object.data())), length(object.length()), index(index)
	{
	}

	~ObjectIterator()
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
		return data == rhs.data && index == rhs.index;
	}

	bool operator!=(const ObjectIterator& rhs) const
	{
		return !operator==(rhs);
	}

	/**
	 * @brief Accessor returns a copy for non-pointer-type elements
	 */
	template <typename T = ElementType>
	typename std::enable_if<!std::is_pointer<T>::value, const ElementType>::type operator*() const
	{
		return readValue(data + index);
	}

	/**
	 * @brief Accessor returns a reference for pointer-type elements
	 */
	template <typename T = ElementType>
	typename std::enable_if<std::is_pointer<T>::value, const typename std::remove_pointer<ElementType>::type&>::type
	operator*() const
	{
		auto ptr = data[index];
		return ptr ? *ptr : std::remove_pointer<ElementType>::type::empty();
	}

private:
	pointer data{};
	size_t length{0};
	unsigned index{0};
};

} // namespace FSTR
