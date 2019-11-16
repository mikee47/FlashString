/**
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
 * You should have received a copy of the GNU General Public License along with FlashString.
 * If not, see <https://www.gnu.org/licenses/>.
 *
 * @author: 2018 - Mikee47 <mike@sillyhouse.net>
 *
 ****/

#pragma once

#include "Object.hpp"

/**
 * @brief Declare a global Vector
 */
#define DECLARE_FSTR_VECTOR(name, ObjectType) extern const FSTR::Vector<ObjectType>& name;

/**
 * @brief Define a Vector (size calculated) with global reference
 */
#define DEFINE_FSTR_VECTOR(name, ObjectType, ...)                                                                      \
	DEFINE_FSTR_VECTOR_DATA(FSTR_DATA_NAME(name), ObjectType, __VA_ARGS__);                                            \
	DEFINE_FSTR_REF_NAMED(name, FSTR::Vector<ObjectType>);

/**
 * @brief Define a Vector (size calculated) with local reference
 */
#define DEFINE_FSTR_VECTOR_LOCAL(name, ObjectType, ...)                                                                \
	DEFINE_FSTR_VECTOR_DATA_LOCAL(FSTR_DATA_NAME(name), ObjectType, __VA_ARGS__);                                      \
	static DEFINE_FSTR_REF_NAMED(name, FSTR::Vector<ObjectType>);

/**
 * @brief Define a Vector (size provided) with global reference
 */
#define DEFINE_FSTR_VECTOR_SIZED(name, ObjectType, size, ...)                                                          \
	DEFINE_FSTR_VECTOR_DATA_SIZED(FSTR_DATA_NAME(name), ObjectType, size, __VA_ARGS__);                                \
	DEFINE_FSTR_REF_NAMED(name, FSTR::Vector<ObjectType>);

/**
 * @brief Define a Vector (size provided) with local reference
 */
#define DEFINE_FSTR_VECTOR_SIZED_LOCAL(name, ObjectType, size, ...)                                                    \
	DEFINE_FSTR_VECTOR_DATA_SIZED_LOCAL(FSTR_DATA_NAME(name), ObjectType, size, __VA_ARGS__);                          \
	static DEFINE_FSTR_REF_NAMED(name, Vector<ObjectType>);

/**
 * @brief Define a Vector data structure, global (non-static)
 */
#define DEFINE_FSTR_VECTOR_DATA(name, ObjectType, ...)                                                                 \
	DEFINE_FSTR_VECTOR_DATA_SIZED(name, ObjectType, sizeof((const void* []){__VA_ARGS__}) / sizeof(void*), __VA_ARGS__)

/**
 * @brief Define a Vector data structure, local (static)
 */
#define DEFINE_FSTR_VECTOR_DATA_LOCAL(name, ObjectType, ...)                                                           \
	static DEFINE_FSTR_VECTOR_DATA(name, ObjectType, __VA_ARGS__)

/**
 * @brief Define a Vector data structure (size provided), global scope
 * @note Use in situations where the array size cannot be automatically calculated,
 * such as when combined with inline Strings via FS()
 */
#define DEFINE_FSTR_VECTOR_DATA_SIZED(name, ObjectType, size, ...)                                                     \
	constexpr const struct {                                                                                           \
		FSTR::ObjectBase object;                                                                                       \
		const ObjectType* data[size];                                                                                  \
	} name PROGMEM = {{sizeof(name.data)}, __VA_ARGS__};                                                               \
	FSTR_CHECK_STRUCT(name);

/**
 * @brief Define a Vector data structure (size provided), local scope
 */
#define DEFINE_FSTR_VECTOR_DATA_SIZED_LOCAL(name, ObjectType, size, ...)                                               \
	static DEFINE_FSTR_VECTOR_DATA_SIZED(name, ObjectType, size, __VA_ARGS__)

namespace FSTR
{
/**
 * @brief Class to access a Vector of objects stored in flash
 */
template <class ObjectType> class Vector : public Object<Vector<ObjectType>, ObjectType*>
{
public:
	const ObjectType& valueAt(unsigned index) const
	{
		if(index < this->length()) {
			return *this->data()[index];
		} else {
			return ObjectType::empty();
		}
	}

	const ObjectType& operator[](unsigned index) const
	{
		return valueAt(index);
	}
};

} // namespace FSTR
