/**
 * FlashMemoryStream.cpp
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
 * @author: 23 Oct 2018 - mikee47 <mike@sillyhouse.net>
 *
 ****/

#include "include/FlashMemoryStream.h"

uint16_t FlashMemoryStream::readMemoryBlock(char* data, int bufSize)
{
	if(flashread) {
		return flashString.readFlash(readPos, data, bufSize);
	} else {
		return flashString.read(readPos, data, bufSize);
	}
}

int FlashMemoryStream::seekFrom(int offset, unsigned origin)
{
	size_t newPos;
	switch(origin) {
	case SEEK_SET:
		newPos = offset;
		break;
	case SEEK_CUR:
		newPos = readPos + offset;
		break;
	case SEEK_END:
		newPos = flashString.length() + offset;
		break;
	default:
		return -1;
	}

	if(newPos > flashString.length()) {
		return -1;
	}

	readPos = newPos;
	return readPos;
}
