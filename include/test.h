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

// Definitions of test framework
#ifndef EV3_TEST_H
#define EV3_TEST_H
#include <ev3Private.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

void ev3TestInit(char* paths[], void* addrs[]);
int ev3TestOpen(char* pathname);
int ev3TestWrite(int fd, void* buf, size_t count);
void * ev3TestMmap(int fd);
int ev3TestExitCode();
void ev3TestAssert();

// Super-globals
extern bool result;
extern size_t expectedLen;
extern uint8_t expected[6];

#endif // EV3_TEST_H