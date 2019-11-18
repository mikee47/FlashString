/**
 * ObjectBase.hpp - Definitions and macros common to all object types
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
 * @author: Nov 2019 - Mikee47 <mike@sillyhouse.net>
 *
 ****/

#pragma once

#include "config.hpp"

namespace FSTR
{
class ObjectBase
{
public:
	/**
	 * @brief Get the length of the object data in bytes
	 */
	FSTR_INLINE uint32_t length() const
	{
		return getObjectLength(getObjectPtr());
	}

	/**
	 * @brief Get the object data size in bytes
	 * @note Always an integer multiple of 4 bytes
	 */
	FSTR_INLINE uint32_t size() const
	{
		return ALIGNUP(getObjectLength(getObjectPtr()));
	}

	/**
	 * @brief Cast to a different object type
	 * @note example:
	 *
	 * 		fstr.as<Array<int>>();
	 */
	template <class ObjectType> FSTR_INLINE constexpr const ObjectType& as() const
	{
		return *static_cast<const ObjectType*>(this);
	}

	/**
	 * @brief Get a pointer to the flash data
	 */
	FSTR_INLINE const uint8_t* data() const
	{
		return getObjectData(getObjectPtr());
	}

	/**
	 * @brief Read contents of a String into RAM
	 * @param offset Zero-based offset from start of flash data to start reading
	 * @param buffer Where to store data
	 * @param count How many bytes to read
	 * @retval size_t Number of bytes actually read
	 */
	size_t read(size_t offset, void* buffer, size_t count) const
	{
		auto ptr = getObjectPtr();
		auto len = getObjectLength(ptr);
		if(offset >= len) {
			return 0;
		}

		count = std::min(len - offset, count);
		memcpy_P(buffer, getObjectData(ptr) + offset, count);
		return count;
	}

	/**
	 * @brief Read contents of a String into RAM, using flashread()
	 * @param offset Zero-based offset from start of flash data to start reading
	 * @param buffer Where to store data
	 * @param count How many bytes to read
	 * @retval size_t Number of bytes actually read
	 * @note PROGMEM data is accessed via the CPU data cache, so to avoid degrading performance
	 * you can use this method to read data directly from flash memory.
	 * This is appropriate for infrequently accessed data, especially if it is large.
	 * For example, if storing content using `IMPORT_FSTR` instead of SPIFFS then it
	 * is generally better to avoid contaminating the cache.
	 * @see See also `FlashMemoryStream` class.
	 */
	size_t readFlash(size_t offset, void* buffer, size_t count) const;

	FSTR_INLINE bool isCopy() const
	{
		return flashLength_ & copyBit;
	}

	/* Member data must be public for initialisation to work but DO NOT ACCESS DIRECTLY !! */

	uint32_t flashLength_;
	// const uint8_t data[]

protected:
	static const ObjectBase empty_;

	/*
	 * @brief Make a 'copy' of this object
	 */
	void copy(const ObjectBase& obj)
	{
		if(obj.isCopy() || obj.flashLength_ == 0) {
			flashLength_ = obj.flashLength_;
		} else {
			flashLength_ = reinterpret_cast<uint32_t>(&obj) | copyBit;
		}
	}

private:
	static constexpr uint32_t copyBit = 0x80000000U;

	/**
	 * @brief Get a pointer to the read flash object as it may be a copy
	 */
	const ObjectBase* getObjectPtr() const;

	FSTR_INLINE uint32_t getObjectLength(const ObjectBase* ptr) const
	{
		assert(ptr != nullptr && !ptr->isCopy());
		return ptr->flashLength_;
	}

	FSTR_INLINE const uint8_t* getObjectData(const ObjectBase* ptr) const
	{
		return reinterpret_cast<const uint8_t*>(&ptr->flashLength_ + 1);
	}
};

}; // namespace FSTR
