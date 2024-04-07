/****
 * ObjectBase.hpp - POD base class type for defining data structures
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
 * @author: Nov 2019 - Mikee47 <mike@sillyhouse.net>
 *
 ****/

#pragma once

#include "config.hpp"

namespace FSTR
{
/**
 * @brief Used when defining data structures
 * @note Should not be used directly, use appropriate Object methods instead
 */
class ObjectBase
{
public:
	/**
	 * @brief Get the length of the object data in bytes
	 */
	size_t length() const;

	/**
	 * @brief Get the object data size in bytes
	 * @note Always an integer multiple of 4 bytes
	 */
	FSTR_INLINE size_t size() const
	{
		return ALIGNUP4(length());
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
	const uint8_t* data() const;

	/**
	 * @brief Read contents of a String into RAM
	 * @param offset Zero-based offset from start of flash data to start reading
	 * @param buffer Where to store data
	 * @param count How many bytes to read
	 * @retval size_t Number of bytes actually read
	 */
	size_t read(size_t offset, void* buffer, size_t count) const;

	/**
	 * @brief Read contents of a String into RAM, using flashread()
	 * @param offset Zero-based offset from start of flash data to start reading
	 * @param buffer Where to store data
	 * @param count How many bytes to read
	 * @retval size_t Number of bytes actually read
	 * @see See also `FlashMemoryStream` class.
	 *
	 * PROGMEM data is accessed via the CPU data cache, so to avoid degrading performance
	 * you can use this method to read data directly from flash memory.
	 * This is appropriate for infrequently accessed data, especially if it is large.
	 * For example, if storing content using `IMPORT_FSTR` instead of SPIFFS then it
	 * is generally better to avoid contaminating the cache.
	 */
	size_t readFlash(size_t offset, void* buffer, size_t count) const;

	FSTR_INLINE bool isCopy() const
	{
		return (flashLength_ & copyBit) != 0;
	}

	/**
	 * @brief Indicates an invalid String, used for return value from lookups, etc.
	 * @note A real String can be zero-length, but it cannot be null
	 */
	FSTR_INLINE bool isNull() const
	{
		return flashLength_ == lengthInvalid;
	}

	/* Member data must be public for initialisation to work but DO NOT ACCESS DIRECTLY !! */

	const uint32_t flashLength_;
	// const uint8_t data[]

protected:
	static const ObjectBase empty_;
	static constexpr uint32_t copyBit = 0x80000000U;	   ///< Set to indicate copy
	static constexpr uint32_t lengthInvalid = copyBit | 0; ///< Indicates null string in a copy
};

} // namespace FSTR
