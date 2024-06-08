/****
 * config.hpp - System-specific configuration
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
 * @author: 2018 - Mikee47 <mike@sillyhouse.net>
 *
 ****/

#pragma once

#include <FakePgmSpace.h>
#include <cassert>
#include <stringutil.h>

#define FSTR_INLINE __attribute__((always_inline)) inline
#define FSTR_NOINLINE __attribute__((noinline))
#define FSTR_ALIGNED __attribute__((aligned(4)))
#define FSTR_PACKED __attribute__((packed))

#ifdef __clang__
// Clang has a habit of throwing stuff away we want
#define FSTR_VOLATILE volatile
#else
#define FSTR_VOLATILE
#endif

#define FSTR_VA_NARGS(type_, ...) (sizeof((const type_[]){__VA_ARGS__}) / sizeof(type_))

#ifndef ALIGNUP4
/**
 * @brief Align a size up to the nearest word boundary
 */
#define ALIGNUP4(n) (((n) + 3) & ~3)
#endif
