/**
 * speed.cpp
 *
 * Copyright 2024 mikee47 <mike@sillyhouse.net>
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
 * @author: Apr 2024 - mikee47 <mike@sillyhouse.net>
 *
 ****/

#include <SmingTest.h>
#include "data.h"

class SpeedTest : public TestGroup
{
public:
	SpeedTest() : TestGroup(_F("Speed"))
	{
	}

	template <typename T> static void __noinline profile_for_loop(const T& object)
	{
		for(unsigned i = 0; i < object.length(); ++i) {
			sum(object.valueAt(i));
		}
	}

	template <typename T> static void __noinline profile_iterator(const T& object)
	{
		for(auto value : object) {
			sum(value);
		}
	}

	template <typename T, typename U> static void __noinline profile_indexOf(const T& object, const U& value)
	{
		total += object.indexOf(value);
	}

	template <typename T, typename U> static void __noinline profile_lookup(const T& object, const U& value)
	{
		sum(object[value]);
	}

	void timeit(Delegate<void()> callback, int expectedTotal)
	{
		total = 0;
		OneShotFastUs timer;
		callback();
		auto elapsed = timer.elapsedTime();
		Serial << _F("Elapsed ") << String(elapsed) << endl;
		REQUIRE_EQ(total, expectedTotal);
	}

	static void sum(int value)
	{
		total += value;
	}

	static void sum(const FSTR::String& value)
	{
		total += value.length();
	}

	void execute() override
	{
		Serial << _F("Array<int> has ") << largeIntArray.length() << _F(" elements.") << endl;
		size_t dataSize{0};
		for(auto& s : largeStringVector) {
			dataSize += s.size();
		}
		Serial << _F("Vector<String> has ") << largeStringVector.length() << _F(" elements, referencing ") << dataSize
			   << " bytes of string data." << endl;
		Serial << _F("Map<int, String> has ") << largeStringMap.length() << _F(" elements.") << endl;

		// Fill cache so comparison is fair
		profile_iterator(largeIntArray);

		TEST_CASE("Array<int> for-loop")
		{
			timeit([]() { profile_for_loop(largeIntArray); }, 61438500);
		}

		TEST_CASE("Array<int> iterator")
		{
			timeit([]() { profile_iterator(largeIntArray); }, 61438500);
		}

		TEST_CASE("Array<int>.indexOf")
		{
			timeit([]() { profile_indexOf(largeIntArray, 122877); }, 999);
		}

		// Fill cache so comparison is fair
		profile_iterator(largeStringVector);

		TEST_CASE("Vector<String> for-loop")
		{
			timeit([]() { profile_for_loop(largeStringVector); }, 2279);
		}

		TEST_CASE("Vector<String> iterator")
		{
			timeit([]() { profile_iterator(largeStringVector); }, 2279);
		}

		TEST_CASE("Vector<String> indexOf(const char*)")
		{
			timeit([]() { profile_indexOf(largeStringVector, _F("Components/*/index")); }, 366);
		}

		TEST_CASE("Vector<String> indexOf(String)")
		{
			timeit([]() { profile_indexOf(largeStringVector, F("Components/*/index")); }, 366);
		}

		// Fill cache so comparison is fair
		profile_iterator(largeStringMap);

		TEST_CASE("Map<int, String> for-loop")
		{
			timeit([]() { profile_for_loop(largeStringMap); }, 2279);
		}

		TEST_CASE("Map<int, String> iterator")
		{
			timeit([]() { profile_iterator(largeStringMap); }, 2279);
		}

		TEST_CASE("Map<int, String> indexOf")
		{
			timeit([]() { profile_indexOf(largeStringMap, 366); }, 366);
		}

		TEST_CASE("Map<int, String> lookup")
		{
			timeit([]() { profile_lookup(largeStringMap, 366); }, 18);
		}
	}

	static int total;
};

int SpeedTest::total;

void REGISTER_TEST(speed)
{
	registerGroup<SpeedTest>();
}
