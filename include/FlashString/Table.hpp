/**
 * Table.hpp - Defines the Table class template and associated macros
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

#include "String.hpp"
#include "ObjectIterator.hpp"

/**
 * @brief Declare a global table of FlashStrings
 */
#define DECLARE_FSTR_TABLE(name, ObjectType) extern const FSTR::Table<ObjectType>& name;

/**
 * @brief Define a FlashString table and reference
 */
#define DEFINE_FSTR_TABLE(name, ObjectType, ...)                                                                       \
	DEFINE_FSTR_TABLE_DATA(FSTR_DATA_NAME(name), ObjectType, __VA_ARGS__);                                             \
	DEFINE_FSTR_REF(name);

#define DEFINE_FSTR_TABLE_LOCAL(name, ObjectType, ...)                                                                 \
	DEFINE_FSTR_TABLE_DATA_LOCAL(FSTR_DATA_NAME(name), ObjectType, __VA_ARGS__);                                       \
	DEFINE_FSTR_REF_LOCAL(name);

#define DEFINE_FSTR_TABLE_SIZED(name, ObjectType, size, ...)                                                           \
	DEFINE_FSTR_TABLE_DATA_SIZED(FSTR_DATA_NAME(name), ObjectType, size, __VA_ARGS__);                                 \
	DEFINE_FSTR_REF(name);

#define DEFINE_FSTR_TABLE_SIZED_LOCAL(name, ObjectType, size, ...)                                                     \
	DEFINE_FSTR_TABLE_DATA_SIZED_LOCAL(FSTR_DATA_NAME(name), ObjectType, size, __VA_ARGS__);                           \
	DEFINE_FSTR_REF_LOCAL(name);

/**
 * @brief Define a structure containing ` data
 */
#define FSTR_TABLE_ARGSIZE(ObjectType, ...) (sizeof((const ObjectType* []){__VA_ARGS__}) / sizeof(void*))
#define DEFINE_FSTR_TABLE_DATA(name, ObjectType, ...)                                                                  \
	DEFINE_FSTR_TABLE_DATA_SIZED(name, ObjectType, FSTR_TABLE_ARGSIZE(ObjectType, __VA_ARGS__), __VA_ARGS__)
#define DEFINE_FSTR_TABLE_DATA_LOCAL(name, ObjectType, ...)                                                            \
	DEFINE_FSTR_TABLE_DATA_SIZED_LOCAL(name, ObjectType, FSTR_TABLE_ARGSIZE(ObjectType, __VA_ARGS__), __VA_ARGS__)

/**
 * @brief Use in situations where the array size cannot be automatically calculated,
 * such as when combined with inline Strings via FS()
 */
#define DEFINE_FSTR_TABLE_DATA_SIZED(name, ObjectType, size, ...)                                                      \
	constexpr const struct {                                                                                           \
		FSTR::Table<ObjectType> object;                                                                                \
		const ObjectType* data[size];                                                                                  \
	} name PROGMEM = {{size}, __VA_ARGS__};
#define DEFINE_FSTR_TABLE_DATA_SIZED_LOCAL(name, ObjectType, size, ...)                                                \
	static DEFINE_FSTR_TABLE_DATA_SIZED(name, ObjectType, size, __VA_ARGS__)

namespace FSTR
{
/**
 * @brief Class to access a table of objects stored in flash
 * @note Data is stored as array of pointers, objects accessed by reference
 */
template <class ObjectType> class Table
{
public:
	using Iterator = ObjectIterator<ObjectType>;

	Iterator begin() const
	{
		return Iterator(head(), tableLength, 0);
	}

	Iterator end() const
	{
		return Iterator(head(), tableLength, tableLength);
	}

	const ObjectType& operator[](unsigned index) const
	{
		if(index < tableLength) {
			auto p = head();
			p += index;
			return **p;
		} else {
			return ObjectType::empty();
		}
	}

	static const Table& empty()
	{
		static const Table PROGMEM empty_{0};
		return empty_;
	}

	unsigned length() const
	{
		return tableLength;
	}

	const ObjectType* const* head() const
	{
		return reinterpret_cast<const ObjectType* const*>(&tableLength + 1);
	}

	/* Private member data */

	const uint32_t tableLength;
	// const ObjectType* entries[];
};

} // namespace FSTR
