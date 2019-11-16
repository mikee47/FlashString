/**
 * Print.cpp - Print support
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
 ****/

#pragma once

#include <Print.h>

namespace FSTR
{
template <class ObjectType>
typename std::enable_if<std::is_class<ObjectType>::value, size_t>::type print(Print& p, const ObjectType& object)
{
	return object.printTo(p);
}

template <typename T> typename std::enable_if<!std::is_class<T>::value, size_t>::type print(Print& p, T value)
{
	return p.print(value);
}

template <typename T> size_t println(Print& p, T value)
{
	size_t size = print(p, value);
	size += p.println();
	return size;
}

} // namespace FSTR
