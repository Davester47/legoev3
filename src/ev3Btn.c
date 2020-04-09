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

// global variables
UI* ui = MAP_FAILED;
int uiFile = -1;

int8_t ev3BtnInit(void) {
  // Open the file
  uiFile = open(UI_DEVICE_NAME, O_RDWR);

  // Check for errors
  if (uiFile < 0) {
    return 0; // ev3Init will free the other source files
  }

  // Do a memory map
  ui = mmap(NULL, sizeof(UI), PROT_READ | PROT_WRITE, MAP_SHARED, uiFile, 0);
  if (ui == MAP_FAILED) {
    return 0; // ev3Init will close uiFile
  }

  return 1; // Everything went well
}

void ev3SetLEDPattern(int8_t pattern) {
  // The driver wants 2 bytes or more, but only cares about the first one
  char msg[2] = {pattern+'0', 0}; // The ones are garbage
  write(uiFile, msg, sizeof(msg));
}

// Returns 1 if the button is pressed,
// and 0 if it's not a button or if it is up.
int8_t ev3BtnRead(int8_t button) {
  // Read the button value from shared memory
  if (button == NO_BUTTON || button > ANY_BUTTON) return 0;
  return button = ui->Pressed[button-1];
}

void ev3BtnFree() {
  if (uiFile >= 0) {
    // It was successfully opened earlier
    close(uiFile);
    uiFile = -1;
  }
  if (ui != MAP_FAILED) {
    munmap(ui, sizeof(UI));
    ui = MAP_FAILED;
  }
}