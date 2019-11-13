/**
 * String.cpp - non-inline code
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

#include <esp_spi_flash.h>
#include "include/FlashString/String.h"

namespace FSTR
{
size_t String::readFlash(size_t offset, void* buffer, size_t bytesToRead) const
{
	if(offset >= flashLength) {
		return 0;
	}

	auto count = std::min(flashLength - offset, bytesToRead);
	auto addr = flashmem_get_address(reinterpret_cast<const uint8_t*>(data()) + offset);
	return flashmem_read(buffer, addr, count);
}

bool String::isEqual(const char* cstr, size_t len) const
{
	// Unlikely we'd want an empty flash string, but check anyway
	if(cstr == nullptr) {
		return flashLength == 0;
	}
	// Don't use strcmp as our data may contain nuls
	if(len == 0) {
		len = strlen(cstr);
	}
	if(len != flashLength) {
		return false;
	}
	LOAD_FSTR(buf, *this);
	return memcmp(buf, cstr, len) == 0;
}

bool String::isEqual(const String& str) const
{
	if(data() == str.data()) {
		return true;
	}
	if(flashLength != str.flashLength) {
		return false;
	}
	return memcmp_aligned(data(), str.data(), flashLength) == 0;
}

} // namespace FSTR
