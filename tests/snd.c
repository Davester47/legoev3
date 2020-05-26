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
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int ev3TestOpen(char* pathname) {
  static int fd = 10;
  if (!strcmp(pathname, SOUND_DEVICE_NAME)) {
    return 1; // fd for the sound file
  } else {
    // Return a unique garbage value for the motor, input, etc. files
    return fd++;
  }
}

uint8_t expected[6] = {0,0,0,0,0,0};
size_t expectedLen = 0;
bool result = false;
int ev3TestWrite(int fd, void* buf, size_t count) {
  // compare received string with the expected one
  uint8_t* buffer = (uint8_t*)buf;
  if (fd == 1) {
    // Then it is ev3Snd.c
    // Compare the data
    int i = 0;
    while((i < count) && (i < expectedLen)) {
      result = (expected[i] == buffer[i]);
      if (result == false) {
        break;
      }
      i++;
    }
  }
  return count;
}

SOUND sound;
void * ev3TestMmap(int fd) {
  return &sound;
}

int exitCode = 0;
void ev3TestAssert() {
  if (!result) {
    printf("FAIL\n");
    exitCode = 1;
  } else {
    printf("OK\n");
  }
}

int main(int argc, char *argv[]) {
  ev3SndInit();
  // Test ev3SndStop
  expectedLen = 1;
  expected[0] = BREAK; // All other bytes are NULL
  printf("Testing ev3SndStop... ");
  ev3SndStop();
  ev3TestAssert();

  // Test ev3SndTone
  expectedLen = 6;
  expected[0] = TONE;
  expected[1] = 99; // level
  expected[2] = 0x24; // hertz lower byte
  expected[3] = 0x01; // hertz higher byte
  expected[4] = 0x20; // msecs lower byte
  expected[5] = 0x01; // msecs higher byte
  printf("Testing ev3SndTone... ");
  ev3SndTone(0x124, 0x120, 99);
  ev3TestAssert();

  printf("Testing ev3SndReady(0)... ");
  sound.Status = BUSY;
  result = !ev3SndReady(0); // It should return false
  sound.Status = OK;
  result = result && ev3SndReady(0); // Should return true
  ev3TestAssert();

  printf("Testing ev3SndReady(-1)... ");
  result = ev3SndReady(-1); // Should return immediately
  ev3TestAssert();

  printf("Testing ev3SndReady(500)... ");
  sound.Status = BUSY;
  result = !ev3SndReady(1); // Should return after about a second
  ev3TestAssert();

  ev3SndFree();
  return exitCode;
}