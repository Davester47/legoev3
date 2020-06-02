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

static UI ui;

int main() {
  char* paths[] = {
    UI_DEVICE_NAME,
    NULL // Always has to end in NULL
  };
  void* ptrs[] = {
    &ui,
    NULL
  };

  ev3TestInit(paths, ptrs);
  if (!ev3BtnInit()) {
    printf("Failed to initialize UI module.\n");
    return 1;
  }

  expectedLen = 2;
  expected[0] = LED_RED + '0';
  expected[1] = 0; // Driver doesn't care about it
  printf("Testing ev3SetLEDPattern... ");
  ev3SetLEDPattern(LED_RED);
  ev3TestAssert();

  printf("Testing ev3BtnRead... ");
  ui.Pressed[UP_BUTTON-1] = true;
  result = ev3BtnRead(UP_BUTTON);
  ui.Pressed[UP_BUTTON-1] = false;
  result = result && !ev3BtnRead(UP_BUTTON);
  ev3TestAssert();

  return ev3TestExitCode();
}