/****
 * MapPrinter.cpp - Print support for maps
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
 ****/

#pragma once

#include "Print.hpp"

namespace FSTR
{
/**
 * @brief Class template to provide a simple way to print the contents of a Map
 * @note Used by Map::printTo() method
 */
template <class MapType> class MapPrinter
{
public:
	MapPrinter(const MapType& map) : map(map)
	{
	}

	size_t printTo(Print& p) const
	{
		size_t count = 0;

		count += p.println("{");
		for(unsigned i = 0; i < map.length(); ++i) {
			count += p.print("  ");
			count += map.valueAt(i).printTo(p);
			count += p.println();
		}
		count += p.print("}");

		return count;
	}

private:
	const MapType& map;
};

} // namespace FSTR
