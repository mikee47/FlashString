/**
 * array.cpp
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
 * @author: Nov 2019 - mikee47 <mike@sillyhouse.net>
 *
 ****/

#include <SmingTest.h>
#include "data.h"

namespace
{
IMPORT_FSTR_ARRAY(custom_bin, char, COMPONENT_PATH "/files/custom.bin");

enum class Fruit {
	bad,
	orange,
	pear,
	kiwi_fruit,
};

struct Size {
	size_t cx, cy, cz;
};

String toString(Fruit fruit)
{
	switch(fruit) {
	case Fruit::bad:
		return F("BAD");
	case Fruit::orange:
		return F("orange");
	case Fruit::pear:
		return F("pear");
	case Fruit::kiwi_fruit:
		return F("kiwi fruit");
	default:
		return nullptr;
	}
}

struct Item {
	Fruit kind;
	unsigned count;
	Size size;

	size_t printTo(Print& p) const
	{
		return p.printf("%s=%u, %ux%ux%u cm", toString(kind).c_str(), count, size.cx, size.cy, size.cz);
	}
};

// clang-format off
DEFINE_FSTR_ARRAY(basket, Item,
		{Fruit::orange, 12, {15, 15, 15}},
		{Fruit::pear, 1, {20, 10, 10}},
		{Fruit::kiwi_fruit, 4, {5, 5, 5}})
// clang-format on

} // namespace

class ArrayTest : public TestGroup
{
public:
	ArrayTest() : TestGroup(_F("Array"))
	{
	}

	void execute() override
	{
		TEST_CASE("print uint8_t[]")
		{
			Serial << externalFSTR1.as<FSTR::Array<uint8_t>>() << endl;
		}

		TEST_CASE("print uint32_t[]")
		{
			Serial << externalFSTR1.as<FSTR::Array<uint32_t>>() << endl;
		}

		TEST_CASE("iterate uint32_t[]")
		{
			for(auto c : externalFSTR1.as<FSTR::Array<uint32_t>>()) {
				Serial.print(c, HEX);
				Serial.print(',');
			}
			Serial.println();
		}

		TEST_CASE("print char[]")
		{
			Serial << externalFSTR1.as<FSTR::Array<char>>() << endl;
		}

		TEST_CASE("iterate double[]")
		{
			Serial.print("{");
			for(auto f : doubleArray) {
				Serial << f << ", ";
			}
			Serial.println(" }");
		}

		TEST_CASE("for-loop double[]")
		{
			Serial << _F("Array[double] length = ") << doubleArray.length() << _F(", size = ") << doubleArray.size()
				   << endl;
			for(unsigned i = 0; i < doubleArray.length(); ++i) {
				Serial << _F("   arr[") << i << _F("] = ") << doubleArray[i] << endl;
			}
		}

		TEST_CASE("print double[]")
		{
			Serial << doubleArray << endl;
		}

		TEST_CASE("iterate int64_t[]")
		{
			//	FSTR::println(Serial, int64Array);
			for(auto v : int64Array) {
				Serial.print(v, HEX);
				Serial.print(", ");
			}
			Serial.println();
		}

		TEST_CASE("Multi-dimensional array")
		{
			FSTR::println(Serial, tableArray);

			Serial.println("  iterator: {");
			for(auto row : tableArray) {
				Serial.print("    { ");
				for(auto v : row.values) {
					Serial << v << ", ";
				}
				Serial.println("},");
			}
			Serial.println(" }");
		}

		TEST_CASE("IMPORT_FSTR_ARRAY")
		{
			Serial << custom_bin << endl;
		}

		TEST_CASE("Iterate struct with class enum")
		{
			for(auto item : basket) {
				Serial << item << endl;
			}

			auto item{basket[1000]};
			Serial << item << endl;
			REQUIRE(item.kind == Fruit::bad);
			REQUIRE(item.count == 0);
		}
	}
};

void REGISTER_TEST(array)
{
	registerGroup<ArrayTest>();
}
