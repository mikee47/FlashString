/**
 * map.cpp
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

class MapTest : public TestGroup
{
public:
	MapTest() : TestGroup(_F("Map"))
	{
	}

	void execute() override
	{
		TEST_CASE("Map of String => String")
		{
			stringMap.printTo(Serial);

			Serial.printf(_F("stringMap[%u]\n"), stringMap.length());

			TEST_CASE("for-loop")
			{
				for(unsigned i = 0; i < stringMap.length(); ++i) {
					auto pair = stringMap.valueAt(i);
					Serial.printf(_F("    stringMap[%u]: '%s' => %u chars\n"), i, String(pair.key()).c_str(),
								  pair.content().length());
				}
			}

			TEST_CASE("lookup")
			{
				Serial.print(_F("stringMap['key1']: "));
				FSTR::println(Serial, stringMap["key1"]);
				REQUIRE(stringMap["key1"]);
				REQUIRE(stringMap["KEY1"]);
				REQUIRE(!stringMap["key1"].content().isNull());
				Serial.print(_F("stringMap['key2']: "));
				FSTR::println(Serial, stringMap["key2"]);
				REQUIRE(stringMap["key2"]);
				REQUIRE(!stringMap["key2"].content().isNull());
				Serial.print(_F("stringMap['non-existent']: "));
				FSTR::println(Serial, stringMap["key20"]);
				REQUIRE(!stringMap["key20"]);
				REQUIRE(stringMap["key20"].content().isNull());
			}
		}

		TEST_CASE("Map[0] as Array<int64>")
		{
			auto& arr = stringMap.valueAt(0).content().as<FSTR::Array<int64_t>>();
			for(auto v : arr) {
				Serial.print(v, HEX);
				Serial.print(", ");
			}
			Serial.println();
			//		FSTR::println(Serial, arr);
		}

		TEST_CASE("Map of enum MapKey => String")
		{
			Serial.printf(_F("enumMap[%u]\n"), enumMap.length());

			Serial.println(_F("  for-loop:"));
			for(unsigned i = 0; i < enumMap.length(); ++i) {
				auto pair = enumMap.valueAt(i);
				Serial.printf(_F("   enumMap[%u] = {%s, %u chars}\r\n"), i, String(pair.key()).c_str(),
							  pair.content().length());
			}
			Serial.println(_F("lookup:"));
			Serial.print(_F("  enumMap[A]: "));
			FSTR::println(Serial, enumMap[KeyA]);
			Serial.print(_F("  enumMap[B]: "));
			FSTR::println(Serial, enumMap[KeyB]);
			Serial.print(_F("  enumMap[C]: "));
			FSTR::println(Serial, enumMap[KeyC]);
		}

		TEST_CASE("Map of String => Vector<String>")
		{
			Serial.printf(_F("vectorMap[%u]\n"), vectorMap.length());

			TEST_CASE("Iterator")
			{
				for(auto pair : vectorMap) {
					Serial.printf(_F("  \"%s\" => %u items\r\n"), String(pair.key()).c_str(), pair.content().length());
				}
			}

			TEST_CASE("for-loop")
			{
				for(unsigned i = 0; i < vectorMap.length(); ++i) {
					auto pair = vectorMap.valueAt(i);
					Serial.printf(_F("    vectorMap[%u] = {'%s', %u items}\r\n"), i, String(pair.key()).c_str(),
								  pair.content().length());
				}
			}

			TEST_CASE("lookup")
			{
				auto printTableMapEntry = [&](const char* key) {
					int i = vectorMap.indexOf(key);
					auto pair = vectorMap.valueAt(i);
					Serial.printf(_F("vectorMap[\"%s\"] = %s, %u items\n"), key, pair ? "OK" : "NOT FOUND",
								  pair.content().length());
				};
				printTableMapEntry("key1");
				printTableMapEntry("key2");
			}
		}
	}
};

void REGISTER_TEST(map)
{
	registerGroup<MapTest>();
}
