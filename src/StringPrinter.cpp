/**
 * StringPrinter.cpp
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

#include "include/FlashString/StringPrinter.hpp"
#include "include/FlashString/String.hpp"
#include <Print.h>

namespace FSTR
{
size_t StringPrinter::printTo(Print& p) const
{
	// Print in chunks
	char buffer[256];
	size_t offset = 0;
	size_t totalWriteCount = 0;
	size_t readCount;
	// For small Strings, read via cache
	if(string.length() <= 64) {
		readCount = string.read(0, buffer, sizeof(buffer));
		return p.write(buffer, readCount);
	}
	while((readCount = string.readFlash(offset, buffer, sizeof(buffer))) > 0) {
		auto writeCount = p.write(buffer, readCount);
		totalWriteCount += writeCount;
		if(writeCount != readCount) {
			break;
		}
		offset += readCount;
	}

	return totalWriteCount;
}

} // namespace FSTR
