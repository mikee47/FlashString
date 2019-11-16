/**
 * Utility.hpp - Definitions, common macros and utility functions
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

#include "config.hpp"

namespace FSTR
{
template <typename T> FORCE_INLINE typename std::enable_if<sizeof(T) == 1, T>::type readValue(const T* ptr)
{
	return static_cast<T>(pgm_read_byte(ptr));
}

template <typename T> FORCE_INLINE typename std::enable_if<sizeof(T) == 2, T>::type readValue(const T* ptr)
{
	return static_cast<T>(pgm_read_word(ptr));
}

template <typename T> FORCE_INLINE typename std::enable_if<IS_ALIGNED(sizeof(T)), T>::type readValue(const T* ptr)
{
	return *static_cast<const T*>(ptr);
}

} // namespace FSTR
