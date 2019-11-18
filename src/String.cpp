/**
 * String.cpp
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

#include "include/FlashString/String.hpp"
#include <WString.h>
#include <esp_spi_flash.h>

namespace FSTR
{
bool String::equals(const char* cstr, size_t len) const
{
	// Unlikely we'd want an empty flash string, but check anyway
	if(cstr == nullptr) {
		return length() == 0;
	}
	// Don't use strcmp as our data may contain nuls
	if(len == 0) {
		len = strlen(cstr);
	}
	if(len != length()) {
		return false;
	}
	LOAD_FSTR(buf, *this);
	return memcmp(buf, cstr, len) == 0;
}

bool String::equals(const String& str) const
{
	if(data() == str.data()) {
		return true;
	}
	if(length() != str.length()) {
		return false;
	}
	return memcmp_aligned(data(), str.data(), length()) == 0;
}

/* Arduino String support */

String::operator WString() const
{
	return WString(data(), length());
}

bool String::equals(const WString& str) const
{
	auto len = str.length();
	if(len != length()) {
		return false;
	}
	// @todo optimise memcmp_P then we won't need to load entire String into RAM first
	LOAD_FSTR(buf, *this);
	return memcmp(buf, str.c_str(), len) == 0;
}

bool String::equalsIgnoreCase(const WString& str) const
{
	auto len = str.length();
	if(len != length()) {
		return false;
	}
	LOAD_FSTR(buf, *this);
	return memicmp(buf, str.c_str(), len) == 0;
}

} // namespace FSTR
