/**
 * data.h - Global test objects
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
 * @author: 18 Sep 2018 - mikee47 <mike@sillyhouse.net>
 *
 ****/

#pragma once

#include <FlashString/String.hpp>
#include <FlashString/Array.hpp>
#include <FlashString/Table.hpp>
#include <FlashString/Vector.hpp>
#include <FlashString/Map.hpp>

/**
 * String
 */

#define EXTERNAL_FSTR1_TEXT "This is an external flash string\0two\0three\0four"
DECLARE_FSTR(externalFSTR1);

/**
 * Array
 */

DECLARE_FSTR_ARRAY(doubleArray, double);
DECLARE_FSTR_ARRAY(int64Array, int64_t);

using TableRow_Float_3 = FSTR::TableRow<float, 3>;
DECLARE_FSTR_ARRAY(tableArray, TableRow_Float_3);

/**
 * Vector
 */

DECLARE_FSTR_VECTOR(stringVector, FSTR::String);
DECLARE_FSTR_VECTOR(arrayVector, FSTR::Array<float>);

/**
 * Map
 */

enum MapKey : uint8_t {
	KeyA = 10,
	KeyB = 20,
	KeyC = 123,
};

DECLARE_FSTR_MAP(enumMap, MapKey, FSTR::String);
DECLARE_FSTR_MAP(stringMap, FSTR::String, FSTR::String);
DECLARE_FSTR_MAP(arrayMap, int, FSTR::Array<float>);
DECLARE_FSTR_MAP(vectorMap, FSTR::String, FSTR::Vector<FSTR::String>);

/**
 * Speed
 */
DECLARE_FSTR_ARRAY(largeIntArray, int)
DECLARE_FSTR_VECTOR(largeStringVector, FSTR::String)
DECLARE_FSTR_MAP(largeStringMap, int, FSTR::String)
