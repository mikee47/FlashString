/**
 * custom.h - Test creation of custom objects
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

#include <WString.h>
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

/*
 * Define a base object type using char[]
 */
class CustomObject : public FSTR::Object<CustomObject, char>
{
protected:
	// Helper method so we can pull out text strings more easily
	String readString(size_t offset, size_t len) const
	{
		char buf[len + 1];
		len = read(offset, buf, len);
		buf[len] = '\0';
		return buf;
	}
};

/*
 * Now derive our custom object with appropriate accessor methods
 */
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
};

/*
 * Declare the global object (imported and defined in custom.cpp).
 */
DECLARE_FSTR_OBJECT(customObject, MyCustomObject);
