/****
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
 * You should have received a copy of the GNU General Public License along with this library.
 * If not, see <https://www.gnu.org/licenses/>.
 *
 ****/

#pragma once

#include "Print.hpp"
#include <stringutil.h>

namespace FSTR
{
template <typename T> typename std::enable_if<!std::is_same<T, char>::value, size_t>::type printElement(Print& p, T e)
{
	return print(p, e);
}

template <typename T> typename std::enable_if<std::is_same<T, char>::value, size_t>::type printElement(Print& p, T c)
{
	auto escape = [](char c) -> char {
		switch(c) {
		case '\0':
			return '0';
		case '\'':
			return '\'';
		case '\"':
			return '"';
		case '\?':
			return '?';
		case '\\':
			return '\\';
		case '\a':
			return 'a';
		case '\b':
			return 'b';
		case '\f':
			return 'f';
		case '\n':
			return 'n';
		case '\r':
			return 'r';
		case '\t':
			return 't';
		case '\v':
			return 'v';
		default:
			return '\0';
		}
	};

	char buf[8];
	char* o = buf;
	*o++ = '\'';
	char esc = escape(c);
	if(esc) {
		*o++ = '\\';
		*o++ = esc;
	} else if(isprint(c)) {
		*o++ = c;
	} else {
		*o++ = '\\';
		*o++ = 'x';
		*o++ = hexchar(uint8_t(c) >> 4);
		*o++ = hexchar(uint8_t(c) & 0x0f);
	}
	*o++ = '\'';
	return p.write(buf, o - buf);
}

/**
 * @brief Class template to provide a simple way to print the contents of an array
 * @note Used by Array::printTo() method
 */
template <class ArrayType> class ArrayPrinter : public Printable
{
public:
	ArrayPrinter(const ArrayType& array) : array(array)
	{
	}

	size_t printTo(Print& p) const override
	{
		size_t count = 0;

		count += p.print('{');
		for(unsigned i = 0; i < array.length(); ++i) {
			if(i > 0) {
				count += p.print(", ");
			}
			count += printElement(p, array[i]);
		}
		count += p.print('}');

		return count;
	}

private:
	const ArrayType& array;
};

} // namespace FSTR
