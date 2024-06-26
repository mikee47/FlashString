/**
 * custom.cpp - Test creation of custom objects
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
#include "custom.h"

/*
 * Import the data and define a reference for it.
 * If we don't want `customObject` to be global, use `IMPORT_FSTR_OBJECT_LOCAL`.
 */
IMPORT_FSTR_OBJECT(customObject, MyCustomObject, COMPONENT_PATH "/files/custom.bin");

class CustomTest : public TestGroup
{
public:
	CustomTest() : TestGroup(_F("Custom Objects"))
	{
	}

	void execute() override
	{
		Serial << FS("Custom object:") << _F("Name: ") << customObject.name() << endl;

		Serial << _F("Desc: ") << customObject.description() << endl;

		Serial << _F("Content: ") << customObject.content() << endl;

		unsigned char data[12] = {0x39, 0x87, 0x12, 0x11, 0x27, 0x91, 0x28, 0x79, 0x87, 0xAA, 0x09, 0xD4};
		REQUIRE(customObject.name() == F("Object Name "));
		REQUIRE(customObject.description() == F("Object Description  "));
		REQUIRE(customObject.content().length() == sizeof(data));
		REQUIRE(memcmp_P(data, customObject.content().data(), sizeof(data)) == 0);

		static const struct {
			const FlashString* string;
		} flashData = {FS_PTR("Inline Flash String")};
		Serial.println(*flashData.string);
	}
};

void REGISTER_TEST(custom)
{
	registerGroup<CustomTest>();
}
