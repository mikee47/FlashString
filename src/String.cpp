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
 * You should have received a copy of the GNU General Public License along with this library.
 * If not, see <https://www.gnu.org/licenses/>.
 *
 ****/

#include "include/FlashString/String.hpp"
#include <WString.h>
#include <esp_spi_flash.h>

namespace FSTR
{
bool String::equals(const char* cstr, size_t clen, bool ignoreCase) const
{
	// Unlikely we'd want an empty flash string, but check anyway
	if(cstr == nullptr) {
		return length() == 0;
	}
	auto len = length();
	if(clen != len) {
		return false;
	}
	LOAD_FSTR(buf, *this);
	if(ignoreCase) {
		return memicmp(buf, cstr, len) == 0;
	}
	return memcmp(buf, cstr, len) == 0;
}

bool String::equals(const char* cstr, bool ignoreCase) const
{
	return equals(cstr, cstr ? strlen(cstr) : 0, ignoreCase);
}

bool String::equals(const String& str, bool ignoreCase) const
{
	auto dataptr = data();
	auto strdata = str.data();
	if(dataptr == strdata) {
		return true;
	}
	auto len = length();
	if(len != str.length()) {
		return false;
	}
	if(ignoreCase) {
		LOAD_FSTR(buf, *this);
		return memicmp(dataptr, buf, len) == 0;
	}
	return memcmp_aligned(dataptr, strdata, len) == 0;
}

/* Wiring String support */

String::operator WString() const
{
	return isNull() ? WString() : WString(data(), length());
}

bool String::equals(const WString& str, bool ignoreCase) const
{
	auto len = str.length();
	if(len != length()) {
		return false;
	}
	// @todo optimise memcmp_P then we won't need to load entire String into RAM first
	LOAD_FSTR(buf, *this);
	if(ignoreCase) {
		return memicmp(buf, str.c_str(), len) == 0;
	}
	return memcmp(buf, str.c_str(), len) == 0;
}

} // namespace FSTR
