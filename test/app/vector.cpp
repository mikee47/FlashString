/**
 * test-vector.cpp
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

class VectorTest : public TestGroup
{
public:
	VectorTest() : TestGroup(_F("Vector"))
	{
	}

	void execute() override
	{
		Serial.print("arrayVector: ");
		arrayVector.printTo(Serial);
		Serial.println();

		//

		Serial.println();
		Serial.println(_F("Vector<String>"));

		table.printTo(Serial);
		Serial.println();

		Serial.printf(_F("table[%u]\n"), table.length());

		Serial.println(_F("  iterator:"));
		for(auto& obj : table) {
			Serial.println(obj);
			Serial.printf(_F("    .length() = %u\n"), obj.length());
		}

		Serial.println(_F("  for-loop:"));
		for(unsigned i = 0; i < table.length(); ++i) {
			Serial.printf(_F("   fstr[%u] = "), i);
			auto& content = table[i];
			Serial.println(content);
			Serial.printf(_F("     .length() = %u\n"), content.length());
		}
	}
};

void REGISTER_TEST(vector)
{
	registerGroup<VectorTest>();
}
