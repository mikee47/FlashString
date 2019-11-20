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
 * You should have received a copy of the GNU General Public License along with FlashString.
 * If not, see <https://www.gnu.org/licenses/>.
 *
 ****/

#include "include/FlashString/ObjectBase.hpp"
#include <esp_spi_flash.h>

namespace FSTR
{
const ObjectBase ObjectBase::empty_{ObjectBase::lengthInvalid};
constexpr uint32_t ObjectBase::copyBit;

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

size_t ObjectBase::length() const
{
	if(isNull()) {
		return 0;
	} else if(isCopy()) {
		return reinterpret_cast<const ObjectBase*>(flashLength_ & ~copyBit)->length();
	} else {
		return flashLength_;
	}
}

const uint8_t* ObjectBase::data() const
{
	if(isNull()) {
		// Return a pointer to a valid memory location
		return reinterpret_cast<const uint8_t*>(&flashLength_);
	}

	auto ptr = this;

	if(isCopy()) {
		// Get real object
		ptr = reinterpret_cast<const ObjectBase*>(flashLength_ & ~copyBit);
	}

	// Cannot yet differentiate memory addresses on Host
#ifndef ARCH_HOST
	// Check we've got a real flash pointer
	assert(isFlashPtr(ptr));
#endif

	return reinterpret_cast<const uint8_t*>(&ptr->flashLength_ + 1);
}

void ObjectBase::invalidate()
{
#ifndef ARCH_HOST
	// Illegal on real flash object
	assert(!isFlashPtr(this));
	if(isFlashPtr(this)) {
		return;
	}
#endif
	flashLength_ = lengthInvalid;
}

} // namespace FSTR
