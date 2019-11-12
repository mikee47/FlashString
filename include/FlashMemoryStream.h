/**
 * FlashMemoryStream.h
 *
 * Copyright 2019 mikee47 <mike@sillyhouse.net>
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

#include <Data/Stream/DataSourceStream.h>
#include "FlashString.h"

/** @addtogroup stream
 *  @{
 */

/*
 * FlashMemoryDataStream
 *
 * Provides a stream buffer on flash storage, so it's read-only
 *
 */
class FlashMemoryStream : public IDataSourceStream
{
public:
	/**
	 * @brief Constructor
	 * @param flashString
	 * @param flashread Specify true to read using flashmem functions,
	 * otherwise data is accessed via CPU cache
	 */
	FlashMemoryStream(const FlashString& flashString, bool flashread = true)
		: flashString(flashString), flashread(flashread)
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
		return flashString.length() - readPos;
	}

	uint16_t readMemoryBlock(char* data, int bufSize) override;

	int seekFrom(int offset, unsigned origin) override;

	bool isFinished() override
	{
		return readPos >= flashString.length();
	}

private:
	const FlashString& flashString;
	size_t readPos = 0;
	bool flashread;
};

/** @} */
