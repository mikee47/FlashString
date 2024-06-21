/****
 * Vector.hpp - Defines the Vector class template and associated macros
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
 * @author: 2018 - Mikee47 <mike@sillyhouse.net>
 *
 ****/

#pragma once

#include "Object.hpp"
#include "ArrayPrinter.hpp"

/**
 * @defgroup fstr_vector Vectors
 * @ingroup FlashString
 * @{
 */

/**
 * @brief Declare a global Vector& reference
 * @param name
 * @param ObjectType
 * @note Use `DEFINE_VECTOR` to instantiate the global Object
 */
#define DECLARE_FSTR_VECTOR(name, ObjectType) DECLARE_FSTR_OBJECT(name, FSTR::Vector<ObjectType>)

/**
 * @brief Define a Vector Object with global reference
 * @param name Name of Vector& reference to define
 * @param ObjectType
 * @param ... List of ObjectType* pointers
 * @note Size will be calculated
 */
#define DEFINE_FSTR_VECTOR(name, ObjectType, ...)                                                                      \
	static DEFINE_FSTR_VECTOR_DATA(FSTR_DATA_NAME(name), ObjectType, __VA_ARGS__);                                     \
	DEFINE_FSTR_REF(name)

/**
 * @brief Like DEFINE_FSTR_VECTOR except reference is declared static constexpr
 */
#define DEFINE_FSTR_VECTOR_LOCAL(name, ObjectType, ...)                                                                \
	static DEFINE_FSTR_VECTOR_DATA(FSTR_DATA_NAME(name), ObjectType, __VA_ARGS__);                                     \
	DEFINE_FSTR_REF_LOCAL(name)

/**
 * @brief Define a Vector Object with global reference, specifying the number of elements
 * @param name Name of Vector& reference to define
 * @param ObjectType
 * @param size Number of elements
 * @param ... List of ObjectType* pointers
 * @note Use in situations where the array size cannot be automatically calculated
 */
#define DEFINE_FSTR_VECTOR_SIZED(name, ObjectType, size, ...)                                                          \
	static DEFINE_FSTR_VECTOR_DATA_SIZED(FSTR_DATA_NAME(name), ObjectType, size, __VA_ARGS__);                         \
	DEFINE_FSTR_REF(name)

/**
 * @brief Like DEFINE_FSTR_VECTOR_SIZED except reference is declared static constexpr
 */
#define DEFINE_FSTR_VECTOR_SIZED_LOCAL(name, ObjectType, size, ...)                                                    \
	static DEFINE_FSTR_VECTOR_DATA_SIZED(FSTR_DATA_NAME(name), ObjectType, size, __VA_ARGS__);                         \
	DEFINE_FSTR_REF_LOCAL(name)

/**
 * @brief Define a Vector data structure
 * @param name Name of data structure
 * @param ObjectType
 * @param ... List of ObjectType* pointers
 * @note Size will be calculated
 */
#define DEFINE_FSTR_VECTOR_DATA(name, ObjectType, ...)                                                                 \
	DEFINE_FSTR_VECTOR_DATA_SIZED(name, ObjectType, FSTR_VA_NARGS(ObjectType*, __VA_ARGS__), __VA_ARGS__)

/**
 * @brief Define a Vector data structure and specify the number of elements
 * @param name Name of data structure
 * @param ObjectType
 * @param size Number of elements
 * @param ... List of ObjectType* pointers
 * @note Use in situations where the array size cannot be automatically calculated
 */
#define DEFINE_FSTR_VECTOR_DATA_SIZED(name, ObjectType, size, ...)                                                     \
	constexpr const struct {                                                                                           \
		FSTR::Vector<ObjectType> object;                                                                               \
		const ObjectType* data[size];                                                                                  \
	} FSTR_PACKED name PROGMEM = {{sizeof(ObjectType*) * size}, {__VA_ARGS__}};                                        \
	FSTR_CHECK_STRUCT(name);

namespace FSTR
{
/**
 * @brief Class to access a Vector of objects stored in flash
 * @tparam ObjectType
 */
template <class ObjectType> class Vector : public Object<Vector<ObjectType>, const ObjectType*>
{
public:
	using DataPtrType = const ObjectType* const*;

	template <typename ValueType, typename T = ObjectType>
	typename std::enable_if<!std::is_same<T, String>::value, int>::type indexOf(const ValueType& value) const
	{
		return Object<Vector<ObjectType>, const ObjectType*>::indexOf(value);
	}

	template <typename T = ObjectType>
	typename std::enable_if<std::is_same<T, String>::value, int>::type indexOf(const char* value,
																			   bool ignoreCase = true) const
	{
		auto dataptr = this->data();
		auto len = this->length();
		auto clen = strlen(value);
		for(unsigned i = 0; i < len; ++i) {
			if(unsafeValueAt(dataptr, i).equals(value, clen, ignoreCase)) {
				return i;
			}
		}

		return -1;
	}

	template <typename ValueType, typename T = ObjectType>
	typename std::enable_if<std::is_same<T, String>::value, int>::type indexOf(const ValueType& value,
																			   bool ignoreCase = true) const
	{
		auto dataptr = this->data();
		auto len = this->length();
		for(unsigned i = 0; i < len; ++i) {
			if(unsafeValueAt(dataptr, i).equals(value, ignoreCase)) {
				return i;
			}
		}

		return -1;
	}

	const ObjectType& valueAt(unsigned index) const
	{
		return (index < this->length()) ? this->unsafeValueAt(this->data(), index) : ObjectType::empty();
	}

	const ObjectType& operator[](unsigned index) const
	{
		return valueAt(index);
	}

	/* Arduino Print support */

	ArrayPrinter<Vector> printer() const
	{
		return ArrayPrinter<Vector>(*this);
	}

	size_t printTo(Print& p) const
	{
		return printer().printTo(p);
	}

	FSTR_INLINE static const ObjectType& unsafeValueAt(const DataPtrType dataptr, unsigned index)
	{
		auto ptr = dataptr[index];
		return ptr ? *ptr : ObjectType::empty();
	}
} FSTR_PACKED;

} // namespace FSTR

/** @} */
