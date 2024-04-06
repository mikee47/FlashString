/**
 * vector.cpp
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
			Serial << arrayVector << endl;
			Serial << _F("arrayVector[") << arrayVector.length() << ']' << endl;

			DEFINE_FSTR_ARRAY_LOCAL(vec, float, 1, 2, 3);
			int i = arrayVector.indexOf(vec);
			REQUIRE_EQ(i, 0);
		}

		TEST_CASE("Vector<String>")
		{
			Serial << stringVector << endl;
			Serial << _F("stringVector[") << stringVector.length() << ']' << endl;

			TEST_CASE("iterator")
			{
				for(auto& obj : stringVector) {
					Serial.println(obj);
					Serial << _F("  .length() = ") << obj.length() << endl;
				}
			}

			TEST_CASE("for-loop")
			{
				for(unsigned i = 0; i < stringVector.length(); ++i) {
					auto& content = stringVector[i];
					Serial << _F("stringVector[") << i << "] = " << content << endl;
					Serial << _F("  .length() = ") << content.length() << endl;
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
