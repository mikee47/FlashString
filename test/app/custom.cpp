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
 * You should have received a copy of the GNU General Public License along with FlashString.
 * If not, see <https://www.gnu.org/licenses/>.
 *
 * @author: Nov 2019 - mikee47 <mike@sillyhouse.net>
 *
 ****/

#include <SmingTest.h>
#include <FlashString/Array.hpp>

/*
 * This is the structure for our custom object, the contents of which will be
 * loaded from an external file.
 */
struct MyCustomStruct {
	FSTR::ObjectBase object;
	char name[12];
	char description[20];
	/*
	 * A contained Object. This could be:
	 *
	 * 	- A String
	 * 	- An Array
	 * 	- Another MyCustomObject
	 * 	- Another custom object
	 *
	 * Structures cannot contain pointers when loaded from a file,
	 * so Vectors and Maps aren't possible.
	 */
	FSTR::ObjectBase dataArray;
};

class CustomObject : public FSTR::Object<CustomObject, char>
{
};

class MyCustomObject : public CustomObject
{
public:
	String name() const
	{
		return readString(offsetof(MyCustomStruct, name), sizeof(MyCustomStruct::name));
	}

	String description() const
	{
		return readString(offsetof(MyCustomStruct, description), sizeof(MyCustomStruct::description));
	}

	/*
	 * Access the contained object as uint8_t[]
	 */
	const FSTR::Array<uint8_t>& content() const
	{
		return data()->dataArray.as<FSTR::Array<uint8_t>>();
	}

	/*
	 * Provide a pointer to the raw data as defined by our structure
	 */
	const MyCustomStruct* data() const
	{
		return reinterpret_cast<const MyCustomStruct*>(CustomObject::data());
	}

private:
	// Helper method so we can pull out the text strings more easily
	String readString(size_t offset, size_t len) const
	{
		char buf[len + 1];
		read(offset, buf, len);
		buf[len] = '\0';
		return buf;
	}
};

/*
 * Import the data and define a symbol for it.
 * Note that the symbol _is_ the data, it's not a pointer or a reference.
 */
IMPORT_FSTR_DATA(customObject, COMPONENT_PATH "/files/custom.bin");
extern "C" const MyCustomObject customObject;

class CustomTest : public TestGroup
{
public:
	CustomTest() : TestGroup(_F("Custom Objects"))
	{
	}

	void execute() override
	{
		Serial.println(FS("Custom object:"));
		Serial.print(_F("Name: "));
		Serial.println(customObject.name());

		Serial.print(_F("Desc: "));
		Serial.println(customObject.description());

		Serial.print(_F("Content: "));
		customObject.content().printTo(Serial);
		Serial.println();

		unsigned char data[12] = {0x39, 0x87, 0x12, 0x11, 0x27, 0x91, 0x28, 0x79, 0x87, 0xAA, 0x09, 0xD4};
		REQUIRE(customObject.name() == _F("Object Name "));
		REQUIRE(customObject.description() == _F("Object Description  "));
		REQUIRE(customObject.content().length() == sizeof(data));
		REQUIRE(memcmp_P(customObject.content().data(), data, sizeof(data)) == 0);
	}
};

void REGISTER_TEST(custom)
{
	registerGroup<CustomTest>();
}
