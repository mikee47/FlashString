/**
 * Table.hpp - Additional definitions to assist with table creation using an Array
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

#include "ArrayPrinter.hpp"

namespace FSTR
{
/**
 * @brief Class template to define the row of a table
 * @tparam ElementType
 * @tparam Columns Number of columns in the table
 * @note Use with an Array Object to construct simple tables
 * Methods provide Object-like access.
 */
template <typename ElementType, size_t Columns> struct TableRow {
	ElementType values[Columns];

	ElementType operator[](size_t index) const
	{
		return values[index];
	}

	size_t length() const
	{
		return Columns;
	}

	size_t printTo(Print& p) const
	{
		return FSTR::ArrayPrinter<TableRow>(*this, ", ").printTo(p);
	}

	static TableRow empty()
	{
		return TableRow{0};
	}
};

} // namespace FSTR
