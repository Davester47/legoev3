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

#include <ev3Private.h>

int analogFile = -1;
ANALOG* analog = MAP_FAILED;

int8_t ev3InInit(void) {
  analogFile = open(ANALOG_DEVICE_NAME, O_RDWR);
  if (analogFile < 0) {
    ev3InFree();
  }

  analog = mmap(NULL, sizeof(ANALOG), PROT_READ | PROT_WRITE, MAP_SHARED, analogFile, 0);
  close(analogFile); // Mappings persist after their files are closed
  analogFile = -1;
  if (analog == MAP_FAILED) {
    ev3InFree();
    return 0;
  }
  return 1; // It's the same as true
}

int16_t ev3InReadAnalogRaw(int8_t portNum) {
  // right shift to make the value for unpressed "fall off"
  return (*analog).Pin6[portNum][(*analog).LogIn[portNum]] >> 8;
}

int8_t ev3InFree(void) {
  if (analog != MAP_FAILED) {
    munmap(analog, sizeof(ANALOG));
  }
  return 1; // Equivalent to true
}