/****
 * Sming Framework Project - Open Source framework for high efficiency native ESP8266 development.
 * Created 2015 by Skurydin Alexey
 * http://github.com/SmingHub/Sming
 * All files of the Sming Core are provided under the LGPL v3 license.
 *
 * FlashString.cpp
 *
 * @author: 2018 - Mikee47 <mike@sillyhouse.net>
 *
 ****/

#include "include/FlashString.h"
#include "include/FlashStringMap.h"
#include <esp_spi_flash.h>

constexpr uint32_t FlashString::zero PROGMEM;
const FlashStringPair FlashStringPair::empty PROGMEM = {nullptr, nullptr};

size_t FlashString::readFlash(size_t offset, void* buffer, size_t bytesToRead) const
{
	if(offset >= flashLength) {
		return 0;
	}

	auto count = std::min(flashLength - offset, bytesToRead);
	auto addr = flashmem_get_address(reinterpret_cast<const uint8_t*>(data()) + offset);
	return flashmem_read(buffer, addr, count);
}

bool FlashString::isEqual(const char* cstr, size_t len) const
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

bool FlashString::isEqual(const FlashString& str) const
{
	if(data() == str.data()) {
		return true;
	}
	if(flashLength != str.flashLength) {
		return false;
	}
	return memcmp_aligned(data(), str.data(), flashLength) == 0;
}
