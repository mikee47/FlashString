/**
 * Object.hpp - Definitions and macros common to all object types
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

#include "config.hpp"
#include "Utility.hpp"
#include "ObjectBase.hpp"
#include "ObjectIterator.hpp"

namespace FSTR
{
template <class ObjectType, typename ElementType> class Object : public ObjectBase
{
public:
	using Iterator = ObjectIterator<ObjectType, ElementType>;

	/*
	 * @brief Copy constructor
	 * @note Objects are usually passed around by reference or as a pointer,
	 * but for ease of use we need a working copy constructor.
	 */
	Object(const Object& obj)
	{
		copy(obj);
	}

	Iterator begin() const
	{
		return Iterator(this->as<ObjectType>(), 0);
	}

	Iterator end() const
	{
		return Iterator(this->as<ObjectType>(), this->length());
	}

	static const ObjectType& empty()
	{
		return empty_.as<ObjectType>();
	}

	/**
	 * @brief Get the length of the array in elements
	 */
	FORCE_INLINE uint32_t length() const
	{
		return ObjectBase::length() / sizeof(ElementType);
	}

	FORCE_INLINE ElementType valueAt(unsigned index) const
	{
		if(index < this->length()) {
			return readValue(this->data() + index);
		} else {
			return ElementType{0};
		}
	}

	/**
	 * @brief Array operator[]
	 */
	FORCE_INLINE ElementType operator[](unsigned index) const
	{
		return this->valueAt(index);
	}

	FORCE_INLINE size_t elementSize() const
	{
		return sizeof(ElementType);
	}

	FORCE_INLINE const ElementType* data() const
	{
		return reinterpret_cast<const ElementType*>(ObjectBase::data());
	}

	/**
	 * @brief Read contents of an Array into RAM
	 * @param index First element to read
	 * @param buffer Where to store data
	 * @param count How many elements to read
	 * @retval size_t Number of elements actually read
	 */
	size_t read(size_t index, ElementType* buffer, size_t count) const
	{
		auto offset = index * sizeof(ElementType);
		count *= sizeof(ElementType);
		return ObjectBase::read(offset, buffer, count) / sizeof(ElementType);
	}

	/**
	 * @brief Read contents of an Array into RAM, using flashread()
	 * @param index First element to read
	 * @param buffer Where to store data
	 * @param count How many elements to read
	 * @retval size_t Number of elements actually read
	 */
	size_t readFlash(size_t index, ElementType* buffer, size_t count) const
	{
		auto offset = index * sizeof(ElementType);
		count *= sizeof(ElementType);
		return ObjectBase::readFlash(offset, buffer, count) / sizeof(ElementType);
	}
};

}; // namespace FSTR
