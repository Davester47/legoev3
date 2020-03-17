/*
 * legoev3 - a simple library for Lego Mindstorms
 * Copyright (C) 2020 David Stumph
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#ifndef EV3_PRIVATE_H
#define EV3_PRIVATE_H

#include "lms2012.h"
#include "lmstypes.h"
#include "bytecodes.h"
#include "ev3.h"
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <stdbool.h>

void ev3Log(const char* message, ...);
int8_t ev3OutInit(void);
int8_t ev3OutFree(void);
int8_t ev3InInit(void);
int8_t ev3InFree(void);
void ev3InUpdateDevCon(void);

#endif // EV3_PRIVATE_H
