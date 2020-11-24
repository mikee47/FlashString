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
 * You should have received a copy of the GNU General Public License along with this library.
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
		TEST_CASE("Vector<Array<float>>")
		{
			arrayVector.printTo(Serial);
			Serial.println();
		}

		TEST_CASE("Vector<String>")
		{
			stringVector.printTo(Serial);
			Serial.println();

			Serial.printf(_F("stringVector[%u]\n"), stringVector.length());

			TEST_CASE("iterator")
			{
				for(auto& obj : stringVector) {
					Serial.println(obj);
					Serial.printf(_F("  .length() = %u\n"), obj.length());
				}
			}

			TEST_CASE("for-loop")
			{
				for(unsigned i = 0; i < stringVector.length(); ++i) {
					Serial.printf(_F("stringVector[%u] = "), i);
					auto& content = stringVector[i];
					Serial.println(content);
					Serial.printf(_F("  .length() = %u\n"), content.length());
				}
			}

			TEST_CASE("lookup")
			{
				int i = stringVector.indexOf(_F("Test string #2"));
				REQUIRE(i == 2);
				i = stringVector.indexOf(_F("Test STRING #2"), false);
				REQUIRE(i == -1);
				i = stringVector.indexOf(_F("Test STRING #2"));
				REQUIRE(i == 2);
				i = stringVector.indexOf(nullptr);
				REQUIRE(i == 1);
				i = stringVector.indexOf("");
				REQUIRE(i == 1);
				i = stringVector.indexOf(String::empty);
				REQUIRE(i == 1);
			}
		}
	}
};

void REGISTER_TEST(vector)
{
	registerGroup<VectorTest>();
}
