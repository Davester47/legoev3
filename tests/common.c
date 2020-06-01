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

#include <test.h>

#define MAX_FD 10

// Global variables
static int numPaths = 0;
static char testPaths[MAX_FD][100]; // Ten names of 100 length should be plenty
static void* ptrs[MAX_FD];
// Revise these limits later if needed. (Almost certainly not)

// Expects a null terminated list of filenames to monitor for testing
// Last element in paths should be NULL
void ev3TestInit(char* paths[], void* addrs[]) {
  // Copy paths into testPaths
  for (int i = 0; paths[i] != NULL; i++) {
    // Copy addrs into pointers
    ptrs[i] = addrs[i];
    int j = 0;
    while (paths[i][j] != '\0') {
      testPaths[i][j] = paths[i][j];
      j++;
    }
    testPaths[i][j] = '\0'; // Set the last byte of each string to NULL
    numPaths++;
  }
}


// TODO: FINSH THESE!!! and FIX snd.c!!!

int ev3TestOpen(char* pathname) {
  // Figure out which string it matches, if any
  int i = 0;
  while (i < MAX_FD) {
    if (!strcmp(pathname, testPaths[i])) {
      return i+1; // fd's start at 1
    }
    i++;
  }
  // No match
  return MAX_FD+1; // So it doesn't conflict with one we care about
}

uint8_t expected[6] = {0,0,0,0,0,0};
size_t expectedLen = 0;
bool result = false;
int ev3TestWrite(int fd, void* buf, size_t count) {
  // compare received string with the expected one
  uint8_t* buffer = (uint8_t*)buf;
  if (fd <= MAX_FD) {
    // Then it is being tested
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

void * ev3TestMmap(int fd) {
  return ptrs[fd-1]; // ptrs = 0 based, fd = 1 based
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

int ev3TestExitCode() {
  return exitCode;
}