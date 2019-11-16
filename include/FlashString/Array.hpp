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
#include "ArrayPrinter.hpp"

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
	DEFINE_FSTR_ARRAY_DATA(FSTR_DATA_NAME(name), ElementType, __VA_ARGS__);                                            \
	DEFINE_FSTR_REF_NAMED(name, FSTR::Array<ElementType>);

/** @brief Define an array for local (static) use
 *  @param name variable to identify the array
 *  @param ElementType
 *  @param elements
 */
#define DEFINE_FSTR_ARRAY_LOCAL(name, ElementType, ...)                                                                \
	DEFINE_FSTR_ARRAY_DATA_LOCAL(FSTR_DATA_NAME(name), ElementType, __VA_ARGS__);                                      \
	static DEFINE_FSTR_REF_NAMED(name, FSTR::Array<ElementType>);

/** @brief Define an array structure
 *  @param name Name to use for data structure
 *  @param ElementType
 *  @param elements
 */
#define DEFINE_FSTR_ARRAY_DATA(name, ElementType, ...)                                                                 \
	constexpr const struct {                                                                                           \
		FSTR::ObjectBase object;                                                                                       \
		ElementType data[sizeof((const ElementType[]){__VA_ARGS__}) / sizeof(ElementType)];                            \
	} ATTR_PACKED name PROGMEM = {{sizeof(name.data)}, {__VA_ARGS__}};                                                 \
	FSTR_CHECK_STRUCT(name);

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
template <typename ElementType> class Array : public Object<Array<ElementType>, ElementType>
{
public:
	ElementType valueAt(unsigned index) const
	{
		if(index >= this->length()) {
			return ElementType{0};
		}

		union {
			uint8_t u8;
			uint16_t u16;
			ElementType elem;
		} buf;

		auto p = this->data() + index;

		switch(sizeof(ElementType)) {
		case 1:
			buf.u8 = pgm_read_byte(p);
			break;
		case 2:
			buf.u16 = pgm_read_word(p);
			break;
		case 4:
		case 8:
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

	/* Arduino Print support */

	/**
	 * @brief Returns a printer object for this array
	 * @note ElementType must be supported by Print
	 */
	ArrayPrinter<Array> printer(const WString& separator = ", ") const
	{
		return ArrayPrinter<Array>(*this, separator);
	}

	size_t printTo(Print& p) const
	{
		return printer().printTo(p);
	}
} ATTR_PACKED;

} // namespace FSTR
