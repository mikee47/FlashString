/**
 * StringPrinter.cpp - Print support for String
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

#include <Printable.h>

namespace FSTR
{
class String;

/**
 * @brief Wrapper class to efficiently print large Strings
 * @note Used by String::printTo() method.
 * Output in chunks to avoid loading the entire content into RAM.
 */
class StringPrinter : public Printable
{
public:
	StringPrinter(const String& string) : string(string)
	{
	}

	size_t printTo(Print& p) const override;

private:
	const String& string;
};

} // namespace FSTR
