/**
 * Object.hpp - Definitions and macros common to all object types
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
 * @author: 2018 - Mikee47 <mike@sillyhouse.net>
 *
 ****/

#pragma once

#include <FakePgmSpace.h>

/**
 * @brief Define a reference to an object
 * @note Used for other objects, not just String
 */
#define DEFINE_FSTR_REF(name)                                                                                          \
	constexpr const decltype(FSTR_DATA_NAME(name).object)& name PROGMEM = FSTR_DATA_NAME(name).object;
#define DEFINE_FSTR_REF_LOCAL(name) static DEFINE_FSTR_REF(name)

/**
 * @brief Provide internal name for generated flash string structures
 */
#define FSTR_DATA_NAME(name) fstr_data_##name

/**
 * @brief Link the contents of a file
 * @note We need inline assembler's `.incbin` instruction to actually import the data.
 * We use a macro STR() so that if required the name can be resolved from a #defined value.
 * @note This provides a more efficient way to read constant (read-only) file data.
 * The file content is bound into firmware image at link time.
 * @note The String object must be referenced or the linker won't emit it.
 * @note Use PROJECT_DIR to locate files in your project's source tree:
 *  		IMPORT_FSTR_DATA(myFlashData, PROJECT_DIR "/files/my_flash_file.txt");
 * Use COMPONENT_PATH within a component.
 * @note No C/C++ symbol is declared, this is type-dependent and must be done separately:
 * 			extern "C" FlashString myFlashData;
 */
#define STR(x) XSTR(x)
#define XSTR(x) #x
#ifdef __WIN32
#define IMPORT_FSTR_DATA(name, file)                                                                                   \
	__asm__(".section .rodata\n"                                                                                       \
			".global _" STR(name) "\n"                                                                                 \
			".def _" STR(name) "; .scl 2; .type 32; .endef\n"                                                          \
			".align 4\n"                                                                                               \
			"_" STR(name) ":\n"                                                                                        \
			".long _" STR(name) "_end - _" STR(name) " - 4\n"                                                          \
			".incbin \"" file "\"\n"                                                                                   \
			"_" STR(name) "_end:\n");
#else
#ifdef ARCH_HOST
#define IROM_SECTION ".rodata"
#else
#define IROM_SECTION ".irom0.text"
#endif
#define IMPORT_FSTR_DATA(name, file)                                                                                   \
	__asm__(".section " IROM_SECTION "\n"                                                                              \
			".global " STR(name) "\n"                                                                                  \
			".type " STR(name) ", @object\n"                                                                           \
			".align 4\n" STR(name) ":\n"                                                                               \
			".long _" STR(name) "_end - " STR(name) " - 4\n"                                                           \
			".incbin \"" file "\"\n"                                                                                   \
			"_" STR(name) "_end:\n");
#endif

namespace FSTR
{
size_t readFlashData(void* dst, const void* src, size_t count);
};
