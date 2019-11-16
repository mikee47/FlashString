/**
 * Object.cpp
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

#include "include/FlashString/Object.hpp"
#include <esp_spi_flash.h>

namespace FSTR
{
const ObjectBase ObjectBase::empty_{0};

size_t ObjectBase::readFlash(size_t offset, void* buffer, size_t count) const
{
	if(offset >= flashLength) {
		return 0;
	}

	count = std::min(flashLength - offset, count);
	auto addr = flashmem_get_address(data() + offset);
	return flashmem_read(buffer, addr, count);
}

} // namespace FSTR
