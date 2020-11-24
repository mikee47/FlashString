/**
 * data.cpp
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

#include "data.h"
#include <WConstants.h>

/**
 * String
 */

DEFINE_FSTR(externalFSTR1, EXTERNAL_FSTR1_TEXT)

/**
 * Array
 */

DEFINE_FSTR_ARRAY(doubleArray, double, PI, 53.0, 100, 1e8, 47);
DEFINE_FSTR_ARRAY(int64Array, int64_t, 1, 2, 3, 4, 5);
DEFINE_FSTR_ARRAY(tableArray, TableRow_Float_3, {1, 2, 3}, {4, 5, 6}, {7, 8, 9});

/**
 * Vector
 */

DEFINE_FSTR_LOCAL(data1, "Test string #1");
DEFINE_FSTR_LOCAL(data2, "Test string #2");
DEFINE_FSTR_VECTOR(stringVector, FSTR::String, &data1, nullptr, &data2);

DEFINE_FSTR_ARRAY_LOCAL(row1, float, 1, 2, 3);
DEFINE_FSTR_ARRAY_LOCAL(row2, float, 4, 5, 6, 7, 8, 9, 10);
DEFINE_FSTR_VECTOR(arrayVector, FSTR::Array<float>, &row1, &row2);

/**
 * Map
 */

DEFINE_FSTR_MAP(arrayMap, int, FSTR::Array<float>, {1, &row1}, {2, &row2});

// Map of `String => String`

DEFINE_FSTR_LOCAL(key1, "key1");
DEFINE_FSTR_LOCAL(key2, "key2");
IMPORT_FSTR_LOCAL(FS_content1, COMPONENT_PATH "/files/content1.txt");
IMPORT_FSTR_LOCAL(FS_content2, COMPONENT_PATH "/files/content2.txt");
DEFINE_FSTR_MAP(stringMap, FSTR::String, FSTR::String, {&key1, &FS_content1}, {&key2, &FS_content2});

DEFINE_FSTR_MAP(enumMap, MapKey, FSTR::String, {KeyA, &FS_content1}, {KeyB, &FS_content2});

// Note the use of FSTR_PTR(), required for GCC 4.8.5 because stringVector is a global reference and therefore not constexpr
DEFINE_FSTR_MAP(vectorMap, FSTR::String, FSTR::Vector<FSTR::String>, {&key1, FSTR_PTR(stringVector)});
