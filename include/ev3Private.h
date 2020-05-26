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
#include <time.h>

#ifdef DEBUG
void ev3Log(const char* message, ...);
#endif // DEBUG
int8_t ev3OutInit(void);
void   ev3OutFree(void);
int8_t ev3BtnInit(void);
void   ev3BtnFree(void);
int8_t ev3InInit(void);
void   ev3InFree(void);
int8_t ev3SndInit();
void   ev3SndFree();
void ev3InUpdateDevCon(void);

#ifdef EV3_TEST
// Code in here only runs as a part of the testsuite
// Reading/writing and shared memory has to be spoofed with header defines

// Each individual test has to define ev3Test*
int ev3TestOpen(char* pathname);
int ev3TestWrite(int fd, void* buf, size_t count);
void * ev3TestMmap(int fd);

#define open(pathname, ...) \
    ev3TestOpen(pathname);

#define write(fd, buf, count) \
    ev3TestWrite(fd, buf, count);

#define close(fd) {} // Nothing

#define mmap(addr, length, prot, flags, fd, ...) \
    ev3TestMmap(fd);

#define munmap(...) {} // Define to nothing

#endif // EV3_TEST
#endif // EV3_PRIVATE_H
