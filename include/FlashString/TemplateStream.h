/**
 * TemplateStream.h
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

#include "Stream.h"
#include <Data/Stream/TemplateStream.h>

namespace FSTR
{
/**
  * @brief      Template Flash memory stream class
  * @ingroup    stream data
  *
  *  @{
 */

class TemplateStream : public ::TemplateStream
{
public:
	/** @brief Create a template stream on top of a flash memory stream
     *  @param  flashString Source data for the stream
     */
	TemplateStream(const String& string) : ::TemplateStream(new FSTR::Stream(string))
	{
	}
};

/** @} */

} // namespace FSTR
