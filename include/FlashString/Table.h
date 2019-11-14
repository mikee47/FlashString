/**
 * Table.h - Defines the Table class template and associated macros
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

#include "String.h"
#include "ObjectIterator.h"

/**
 * @brief Declare a global table of FlashStrings
 */
#define DECLARE_FSTR_TABLE(name, ObjectType) extern const FSTR::Table<ObjectType>& name;

/**
 * @brief Define a FlashString table and reference
 */
#define DEFINE_FSTR_TABLE(name, ObjectType, ...)                                                                       \
	DEFINE_FSTR_TABLE_DATA(FSTR_DATA_NAME(name), ObjectType, __VA_ARGS__);                                             \
	const FSTR::Table<ObjectType>& name PROGMEM = FSTR_DATA_NAME(name).table;

#define DEFINE_FSTR_TABLE_LOCAL(name, ObjectType, ...)                                                                 \
	DEFINE_FSTR_TABLE_DATA_LOCAL(FSTR_DATA_NAME(name), ObjectType, __VA_ARGS__);                                       \
	static const FSTR::Table<ObjectType>& name PROGMEM = FSTR_DATA_NAME(name).table;

/**
 * @brief Cast a pointer to FSTR::Table*
 */
#define FSTR_TABLE_PTR(ObjectType, data_ptr) reinterpret_cast<const FSTR::Table<ObjectType>*>(data_ptr)

/**
 * @brief Define a FSTR::Table& reference using a cast
 */
#define DEFINE_FSTR_TABLE_REF(name, ObjectType, data_name)                                                             \
	const FSTR::Table<ObjectType>& name = *FSTR_TABLE_PTR(ObjectType, &data_name);

/**
 * @brief Define a structure containing table data
 */
#define FSTR_TABLE_ARGSIZE(ObjectType, ...) (sizeof((const ObjectType* []){__VA_ARGS__}) / sizeof(void*))
#define DEFINE_FSTR_TABLE_DATA(name, ObjectType, ...)                                                                  \
	constexpr struct {                                                                                                 \
		FSTR::Table<ObjectType> table;                                                                                 \
		const ObjectType* data[FSTR_TABLE_ARGSIZE(ObjectType, __VA_ARGS__)];                                           \
	} name PROGMEM = {{FSTR_TABLE_ARGSIZE(ObjectType, __VA_ARGS__)}, {__VA_ARGS__}};
#define DEFINE_FSTR_TABLE_DATA_LOCAL(name, ObjectType, ...) static DEFINE_FSTR_TABLE_DATA(name, ObjectType, __VA_ARGS__)

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
