/**
 * ArrayPrinter.cpp - Print support for arrays
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

#include "Print.hpp"

namespace FSTR
{
/**
 * @brief Class template to provide a simple way to print the contents of an array
 * @note Used by Array::printTo() method
 */
template <class ArrayType> class ArrayPrinter : public Printable
{
public:
	ArrayPrinter(const ArrayType& array, const WString& separator) : array(array), separator(separator)
	{
	}

	size_t printTo(Print& p) const override
	{
		size_t count = 0;

		count += p.print("[");
		for(unsigned i = 0; i < array.length(); ++i) {
			if(i > 0) {
				count += p.print(separator);
			}
			count += print(p, array[i]);
		}
		count += p.print("]");

		return count;
	}

private:
	const ArrayType& array;
	WString separator;
};

} // namespace FSTR
