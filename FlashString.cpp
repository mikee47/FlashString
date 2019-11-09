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

#include "FlashString.h"
#include <esp_spi_flash.h>

size_t FlashString::read(size_t offset, void* buffer, size_t bytesToRead, bool readCache) const
{
	if(offset >= flashLength) {
		return 0;
	}

	auto count = std::min(flashLength - offset, bytesToRead);

	if(readCache) {
		memcpy_P(buffer, &flashData[offset], count);
		return count;
	} else {
		auto addr = flashmem_get_address(&flashData[offset]);
		return flashmem_read(buffer, addr, count);
	}
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
	if(flashLength != str.flashLength) {
		return false;
	}
	if(flashData == str.flashData) {
		return true;
	}
	return memcmp_aligned(flashData, str.flashData, flashLength) == 0;
}
