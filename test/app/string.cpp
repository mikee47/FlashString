/**
 * string.cpp
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

class StringTest : public TestGroup
{
public:
	StringTest() : TestGroup(_F("String"))
	{
	}

	void execute() override
	{
		FSTR::String empty;

		TEST_CASE("empty")
		{
			REQUIRE(empty.isCopy() == false);
			REQUIRE(empty == "");
		}

		TEST_CASE("empty copy")
		{
			auto copy1 = empty;
			REQUIRE(copy1.isCopy() == false);
			REQUIRE(copy1 == "");
		}

		TEST_CASE("normal copy")
		{
			auto copy2 = externalFSTR1;
			REQUIRE(copy2.isCopy() == true);
			REQUIRE(copy2 == F(EXTERNAL_FSTR1_TEXT));
		}

		TEST_CASE("inline")
		{
#define INLINE_TEXT "This is an inline\0 FlashString\0\0"
			auto str = FS(INLINE_TEXT);
			REQUIRE(str == F(INLINE_TEXT));
		}

#define DEMO_TEST_TEXT "This is a flash string -\0Second -\0Third -\0Fourth."

		DEFINE_FSTR_LOCAL(demoFSTR1, DEMO_TEST_TEXT);
		DEFINE_FSTR_LOCAL(demoFSTR2, DEMO_TEST_TEXT);

		TEST_CASE("Direct read")
		{
			char buffer[1024];
			auto len = demoFSTR1.readFlash(0, buffer, sizeof(buffer));
			REQUIRE(len == demoFSTR1.length());
			Serial.print("demoFSTR1.read(): ");
			Serial.write(buffer, len);
			Serial.println();
		}

		TEST_CASE("Implicit conversion to WString")
		{
			Serial.print("> demoFSTR1 (print String): ");
			Serial.print('"');
			Serial.print(demoFSTR1);
			Serial.println('"');
			String s = demoFSTR1;
			REQUIRE(s == F(DEMO_TEST_TEXT));
			REQUIRE(s.length() == sizeof(DEMO_TEST_TEXT) - 1);
		}

		TEST_CASE("LOAD_FSTR")
		{
			Serial.print("> demoFSTR1 (print char*): ");
			LOAD_FSTR(test, demoFSTR1);
			Serial.print('"');
			Serial.print(test);
			Serial.println('"');
			REQUIRE(sizeof(test) == ALIGNUP(sizeof(DEMO_TEST_TEXT)));

			Serial.print("> demoFSTR1 (write): ");
			Serial.print('"');
			Serial.write(test, demoFSTR1.length());
			Serial.println('"');
		}

		TEST_CASE("Print external")
		{
			Serial.print("> externalFSTR1 (print): ");
			Serial.print('"');
			Serial.print(externalFSTR1);
			Serial.println('"');
		}

		TEST_CASE("iterator")
		{
			unsigned count = 0;
			Serial.print("FSTR: { ");
			for(auto c : externalFSTR1) {
				Serial.print('\'');
				Serial.print(c);
				Serial.print('\'');
				Serial.print(", ");
				++count;
			}
			Serial.println(" }");
			REQUIRE(count == sizeof(EXTERNAL_FSTR1_TEXT) - 1);
		}

		TEST_CASE("FSTR_ARRAY")
		{
			// Define a FlashString and load it into a stack buffer in one operation (non-reusable!)
			Serial.print("> FSTR_ARRAY: ");
			FSTR_ARRAY(fsarr, DEMO_TEST_TEXT);
			Serial.print('"');
			Serial.write(fsarr, sizeof(fsarr) - 1);
			Serial.println('"');
			REQUIRE(sizeof(fsarr) == ALIGNUP(sizeof(DEMO_TEST_TEXT)));
			REQUIRE(memcmp_P(fsarr, DEMO_TEST_TEXT, sizeof(DEMO_TEST_TEXT)) == 0);
		}

		// Example of array or custom data usage
		TEST_CASE("Custom Array")
		{
			Serial.print("> demoArray1 : {");
			static const struct {
				FSTR::ObjectBase object;
				char data[5];
			} demoArray1 PROGMEM = {{5}, {1, 2, 3, 4, 5}};
			// Use a reference for convenience
			const auto& fstrArray = demoArray1.object.as<FlashString>();
			// fstrArray can be now be used in calls to functions, etc.
			String arr(fstrArray);
			for(unsigned i = 0; i < arr.length(); ++i) {
				if(i > 0) {
					Serial.print(", ");
				}
				Serial.print(arr[i], DEC);
			}
			Serial.println("}");
		}

		// Test equality operators
		TEST_CASE("Equality")
		{
			REQUIRE(demoFSTR1 == demoFSTR2);
			REQUIRE(String(demoFSTR1) == demoFSTR2);
			REQUIRE(demoFSTR1 == String(demoFSTR2));
		}
	}
};

void REGISTER_TEST(string)
{
	registerGroup<StringTest>();
}
