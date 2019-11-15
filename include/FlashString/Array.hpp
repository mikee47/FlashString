/**
 * Array.hpp - Defines the Array class and associated macros
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
 * @author: 2018 - Mikee47 <mike@sillyhouse.net>
 *
 ****/

#pragma once

#include "Object.hpp"

/**
 * @brief Declare a global Array instance
 */
#define DECLARE_FSTR_ARRAY(name, ElementType) extern const FSTR::Array<ElementType>& name;

/** @brief Define an array
 *  @param name variable to identify the array
 *  @param ElementType
 *  @param elements
 *  @note Unlike String, array is not nul-terminated
 */
#define DEFINE_FSTR_ARRAY(name, ElementType, ...)                                                                      \
	static DEFINE_FSTR_ARRAY_DATA(FSTR_DATA_NAME(name), ElementType, {__VA_ARGS__});                                   \
	DEFINE_FSTR_REF(name);

/** @brief Define an array for local (static) use
 *  @param name variable to identify the array
 *  @param ElementType
 *  @param elements
 */
#define DEFINE_FSTR_ARRAY_LOCAL(name, ElementType, ...)                                                                \
	DEFINE_FSTR_ARRAY_DATA_LOCAL(FSTR_DATA_NAME(name), ElementType, {__VA_ARGS__});                                    \
	DEFINE_FSTR_REF_LOCAL(name);

/** @brief Define an array structure
 *  @param name Name to use for data structure
 *  @param ElementType
 *  @param elements
 */
#define FSTR_ARRAY_ARGSIZE(ElementType, ...) (sizeof((const ElementType[])__VA_ARGS__) / sizeof(ElementType))
#define DEFINE_FSTR_ARRAY_DATA(name, ElementType, ...)                                                                 \
	constexpr const struct {                                                                                           \
		FSTR::Array<ElementType> object;                                                                               \
		ElementType data[FSTR_ARRAY_ARGSIZE(ElementType, __VA_ARGS__)];                                                \
	} name PROGMEM = {{FSTR_ARRAY_ARGSIZE(ElementType, __VA_ARGS__)}, __VA_ARGS__};
#define DEFINE_FSTR_ARRAY_DATA_LOCAL(name, ElementType, ...)                                                           \
	static DEFINE_FSTR_ARRAY_DATA(name, ElementType, __VA_ARGS__)

/**
 * @brief Load an Array object into a named local (stack) buffer
 * @note Example:
 *
 * 	DEFINE_FSTR_ARRAY(fsArray, double, 5.33, PI)
 * 	...
 * 	LOAD_FSTR_ARRAY(arr, fsArray)
 * 	printf("arr[0] = %f, %u elements, buffer is %u bytes\n", arr[0], fsArray.length(), sizeof(arr));
 */
#define LOAD_FSTR_ARRAY(name, array)                                                                                   \
	decltype(array)[0] name[(array).size()] __attribute__((aligned(4)));                                               \
	memcpy_aligned(name, (fstr).data(), (fstr).size());

/**
 * @brief Define an Array and load it into a named buffer on the stack
 * @note Equivalent to `ElementType name[] = {a, b, c}` except the buffer is word-aligned
 */
#define FSTR_ARRAY_ARRAY(name, ElementType, ...)                                                                       \
	DEFINE_FSTR_ARRAY_DATA_LOCAL(FSTR_DATA_NAME(name), ElementType, __VA_ARGS__);                                      \
	LOAD_FSTR_ARRAY(name, FSTR_DATA_NAME(name).object)

/** @brief Define an Array containing data from an external file
 *  @param name Name for the object
 *  @param file Absolute path to the file containing the content
 */
#define IMPORT_FSTR_ARRAY(name, ElementType, file)                                                                     \
	IMPORT_FSTR_DATA(name, file)                                                                                       \
	extern "C" const FSTR::Array<ElementType> name;

namespace FSTR
{
/**
 * @brief describes an array of integral values stored in flash memory
 */
template <typename ElementType> class Array
{
public:
	using Iterator = ArrayIterator<Array, ElementType>;

	Iterator begin() const
	{
		return Iterator(*this, 0);
	}

	Iterator end() const
	{
		return Iterator(*this, length());
	}

	static const Array& empty()
	{
		static const Array PROGMEM empty_{0};
		return empty_;
	}

	/**
	 * @brief Get the length of the array in elements
	 */
	uint32_t length() const
	{
		return flashLength;
	}

	/**
	 * @brief Get the number of bytes used to store the Array
	 * @note Always an integer multiple of 4 bytes
	 */
	uint32_t size() const
	{
		return ALIGNUP(flashLength * sizeof(ElementType));
	}

	ElementType valueAt(unsigned index) const
	{
		if(index >= flashLength) {
			return ElementType{0};
		}

		union {
			uint8_t u8;
			uint16_t u16;
			ElementType elem;
		} buf;

		auto p = reinterpret_cast<const ElementType*>(data()) + index;
		switch(sizeof(ElementType)) {
		case 1:
			buf.u8 = pgm_read_byte(p);
			break;
		case 2:
			buf.u16 = pgm_read_word(p);
			break;
		case 4:
			buf.elem = *p;
			break;
		default:
			memcpy_P(&buf, p, sizeof(ElementType));
		}
		return buf.elem;
	}

	ElementType operator[](unsigned index) const
	{
		return valueAt(index);
	}

	/**
	 * @brief Get a pointer to the flash data
	 */
	const ElementType* data() const
	{
		struct Struct {
			uint32_t length;
			ElementType data[1];
		};
		return &reinterpret_cast<const Struct*>(&flashLength)->data[0];
	}

	/**
	 * @brief Read contents of an Array into RAM
	 * @param index First element to read
	 * @param buffer Where to store data
	 * @param count How many elements to read
	 * @retval size_t Number of elements actually read
	 */
	size_t read(size_t index, ElementType* buffer, size_t count) const
	{
		if(index >= flashLength) {
			return 0;
		}

		count = std::min(flashLength - index, count);
		memcpy_P(buffer, data() + index, count * sizeof(ElementType));
		return count;
	}

	/**
	 * @brief Read contents of an Array into RAM, using flashread()
	 * @param index First element to read
	 * @param buffer Where to store data
	 * @param count How many elements to read
	 * @retval size_t Number of elements actually read
	 */
	size_t readFlash(size_t index, ElementType* buffer, size_t count) const
	{
		if(index >= flashLength) {
			return 0;
		}

		count = std::min(flashLength - index, count);
		return readFlashData(buffer, data() + index, count * sizeof(ElementType));
	}

	/* Private member data */

	uint32_t flashLength;
	// const uint8_t data[]
};

} // namespace FSTR
