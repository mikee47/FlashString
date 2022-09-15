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
 * You should have received a copy of the GNU General Public License along with this library.
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
			Serial << stringMap << _F(" stringMap[") << stringMap.length() << ']' << endl;

			TEST_CASE("for-loop")
			{
				for(unsigned i = 0; i < stringMap.length(); ++i) {
					auto pair = stringMap.valueAt(i);
					Serial << _F("    stringMap[") << i << "]: '" << pair.key() << "' => " << pair.content().length()
						   << " chars" << endl;
				}
			}

			TEST_CASE("lookup")
			{
				Serial << _F("stringMap['key1']: ") << stringMap["key1"] << endl;
				REQUIRE(stringMap["key1"]);
				REQUIRE(stringMap["KEY1"]);
				REQUIRE(!stringMap["key1"].content().isNull());
				Serial << _F("stringMap['key2']: ") << stringMap["key2"] << endl;
				REQUIRE(stringMap["key2"]);
				REQUIRE(!stringMap["key2"].content().isNull());
				Serial << _F("stringMap['non-existent']: ") << stringMap["key20"] << endl;
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
				Serial << _F("   enumMap[") << i << "] = {" << pair.key() << ", " << pair.content().length()
					   << " chars}" << endl;
			}
			Serial.println(_F("lookup:"));
			Serial << _F("  enumMap[A]: ") << enumMap[KeyA] << endl;
			Serial << _F("  enumMap[B]: ") << enumMap[KeyB] << endl;
			Serial << _F("  enumMap[C]: ") << enumMap[KeyC] << endl;
		}

		TEST_CASE("Map of String => Vector<String>")
		{
			Serial << _F("vectorMap[") << vectorMap.length() << ']' << endl;

			TEST_CASE("Iterator")
			{
				for(auto pair : vectorMap) {
					Serial << _F("  '") << pair.key() << "' => " << pair.content().length() << " items" << endl;
				}
			}

			TEST_CASE("for-loop")
			{
				for(unsigned i = 0; i < vectorMap.length(); ++i) {
					auto pair = vectorMap.valueAt(i);
					Serial << _F("    vectorMap[") << i << "] = {'" << pair.key() << "', " << pair.content().length()
						   << " items}" << endl;
				}
			}

			TEST_CASE("lookup")
			{
				auto printTableMapEntry = [&](const char* key) {
					int i = vectorMap.indexOf(key);
					auto pair = vectorMap.valueAt(i);

					Serial << _F("vectorMap['") << key << "'] = " << (pair ? "OK" : "NOT FOUND") << ", "
						   << pair.content().length() << " items" << endl;
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
