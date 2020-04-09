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

SOUND* snd = MAP_FAILED;
int sndFile = -1;

int8_t ev3SndInit() {
  // Open the file
  sndFile = open(SOUND_DEVICE_NAME, O_RDWR);
  if (sndFile < 0) {
    return 0; // Opening the file failed
  }
  
  // Map shared memory
  snd = mmap(NULL, sizeof(SOUND), PROT_READ | PROT_WRITE, MAP_SHARED, sndFile, 0);
  if (snd == MAP_FAILED) {
    return 0; // ev3SndFree is called to close the file
  }
  return 1;
}

void ev3SndFree() {
  // Only munmap and close if they were opened earlier
  if (sndFile >= 0) {
    close(sndFile);
    sndFile = -1; // In case this gets called again
  }

  if (snd != MAP_FAILED) {
    munmap(snd, sizeof(SOUND));
  }
}