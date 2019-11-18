/**
 * Stream.hpp
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
 * @author: 23 Oct 2018 - mikee47 <mike@sillyhouse.net>
 *
 ****/

#pragma once

#include "String.hpp"
#include <Data/Stream/DataSourceStream.h>

namespace FSTR
{
/** @addtogroup stream
 *  @{
 */

/*
 * Provides a stream buffer on flash storage (read-only)
 */
class Stream : public IDataSourceStream
{
public:
	/**
	 * @brief Constructor
	 * @param string
	 * @param flashread Specify true to read using flashmem functions, otherwise data is accessed via cache
	 */
	Stream(const String& string, bool flashread = true) : string(string), flashread(flashread)
	{
	}

	StreamType getStreamType() const override
	{
		return eSST_Memory;
	}

	/**
	 * @brief Return the total length of the stream
	 * @retval int -1 is returned when the size cannot be determined
	*/
	int available() override
	{
		return string.length() - readPos;
	}

	uint16_t readMemoryBlock(char* data, int bufSize) override;

	int seekFrom(int offset, unsigned origin) override;

	bool isFinished() override
	{
		return readPos >= string.length();
	}

private:
	const String& string;
	size_t readPos = 0;
	bool flashread;
};

/** @} */

} // namespace FSTR
