/**
 * Stream.cpp
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
 * @author: 23 Oct 2018 - mikee47 <mike@sillyhouse.net>
 *
 ****/

#include "include/FlashString/Stream.hpp"

namespace FSTR
{
uint16_t Stream::readMemoryBlock(char* data, int bufSize)
{
	if(flashread) {
		return object.readFlash(readPos, data, bufSize);
	} else {
		return object.read(readPos, data, bufSize);
	}
}

int Stream::seekFrom(int offset, SeekOrigin origin)
{
	size_t newPos;
	switch(origin) {
	case SeekOrigin::Start:
		newPos = offset;
		break;
	case SeekOrigin::Current:
		newPos = readPos + offset;
		break;
	case SeekOrigin::End:
		newPos = object.length() + offset;
		break;
	default:
		return -1;
	}

	if(newPos > object.length()) {
		return -1;
	}

	readPos = newPos;
	return readPos;
}

} // namespace FSTR
