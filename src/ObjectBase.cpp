/**
 * ObjectBase.cpp
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

#include "include/FlashString/ObjectBase.hpp"
#include <esp_spi_flash.h>

namespace FSTR
{
const ObjectBase ObjectBase::empty_{ObjectBase::lengthInvalid};

bool ObjectBase::operator==(const ObjectBase& other) const
{
	if(this == &other) {
		return true;
	}
	if(length() != other.length()) {
		return false;
	}
	return memcmp(this, &other, sizeof(flashLength_) + size()) == 0;
}

size_t ObjectBase::readFlash(size_t offset, void* buffer, size_t count) const
{
	auto len = length();
	if(offset >= len) {
		return 0;
	}

	count = std::min(len - offset, count);
	auto addr = flashmem_get_address(data() + offset);
	return flashmem_read(buffer, addr, count);
}

size_t ObjectBase::read(size_t offset, void* buffer, size_t count) const
{
	auto len = length();
	if(offset >= len) {
		return 0;
	}

	count = std::min(len - offset, count);
	memcpy_P(buffer, data() + offset, count);
	return count;
}

const uint8_t* ObjectBase::data() const
{
	return reinterpret_cast<const uint8_t*>(&flashLength_ + 1);
}

} // namespace FSTR
