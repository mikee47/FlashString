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
 * You should have received a copy of the GNU General Public License along with FlashString.
 * If not, see <https://www.gnu.org/licenses/>.
 *
 * @author: Nov 2019 - mikee47 <mike@sillyhouse.net>
 *
 ****/

#include <SmingTest.h>
#include "data.h"

class ArrayTest : public TestGroup
{
public:
	ArrayTest() : TestGroup(_F("Array"))
	{
	}

	void execute() override
	{
		externalFSTR1.as<FSTR::Array<uint8_t>>().printTo(Serial);
		Serial.println();
		externalFSTR1.as<FSTR::Array<uint32_t>>().printTo(Serial);
		for(auto c : externalFSTR1.as<FSTR::Array<uint32_t>>()) {
			Serial.print(c, HEX);
			Serial.print(',');
		}
		Serial.println();
		externalFSTR1.as<FSTR::Array<char>>().printTo(Serial);
		Serial.println();

		//

		Serial.println();
		Serial.print(_F("Array of double"));

		Serial.print("  iterator: {");
		for(auto f : doubleArray) {
			Serial.print(f);
			Serial.print(", ");
		}
		Serial.println(" }");

		Serial.printf(_F("Array[double] length = %u, size = %u"), doubleArray.length(), doubleArray.size());
		for(unsigned i = 0; i < doubleArray.length(); ++i) {
			Serial.printf(_F("   arr[%u] = %f\n"), i, doubleArray[i]);
		}

		FSTR::println(Serial, doubleArray);

		//

		Serial.println();
		Serial.print(_F("Multi-dimensional array"));

		FSTR::println(Serial, tableArray);

		Serial.println("  iterator: {");
		for(auto row : tableArray) {
			Serial.print("    { ");
			for(auto v : row.values) {
				Serial.print(v);
				Serial.print(", ");
			}
			Serial.println("},");
		}
		Serial.println(" }");

		//	FSTR::println(Serial, int64Array);
		for(auto v : int64Array) {
			Serial.print(v, HEX);
			Serial.print(", ");
		}
		Serial.println();
	}
};

void REGISTER_TEST(array)
{
	registerGroup<ArrayTest>();
}
