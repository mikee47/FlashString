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

#include <Print.h>
#include <WString.h>

namespace FSTR
{
template <class ArrayType> class ArrayPrinter : public Printable
{
public:
	ArrayPrinter(const ArrayType& array, const WString& separator) : array(array), separator(separator)
	{
	}

	size_t printTo(Print& p) const override;

private:
	const ArrayType& array;
	WString separator;
};

template <class ArrayType> size_t ArrayPrinter<ArrayType>::printTo(Print& p) const
{
	size_t count = 0;

	for(unsigned i = 0; i < array.length(); ++i) {
		if(i > 0) {
			count += p.print(separator);
		}
		count += p.print(array.valueAt(i));
	}

	return count;
}
} // namespace FSTR
