/*
 * libev3 - a simple library for Lego Mindstorms
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

#include <ev3.h>
#include <unistd.h>
#include <time.h>

int main() {
// Always initialise legoev3
  ev3Init();

  // Start a motor connected to port A and set its speed to 100
  // It will ramp up for 1/2 a turn, spin for 3 turns, and slow down for
  // 1/2 a turn

  ev3OutStepPower(OUT_A, 100, 180, 360 * 3, 180, 1);
  ev3OutReady(OUT_A, 10); // Wait for the above operation to complete

  // Output ports can be combined like this:
  ev3OutSpeed(OUT_A | OUT_B, 50);
  ev3OutStart(OUT_A | OUT_B);
  sleep(3);
  ev3OutStop(OUT_A | OUT_B, 1);
  ev3OutReady(OUT_A | OUT_B, 10);

  // Find the first port with a touch sensor:
  int8_t touchPort = IN_1;
  while(touchPort <= IN_4) {
    if (ev3InGetType(touchPort) == TYPE_TOUCH) {
      break;
    }
    touchPort++;
  }

  // Loop for 10 seconds and go while the touch sensor is pressed
  // For some reason this is incredibly laggy, and I don't know why.
  time_t startTime = time(NULL);
  int state = 0;
  while (difftime(time(NULL), startTime) <= 10) {
    int16_t value = ev3InReadAnalogRaw(touchPort);
    if (value && value != state) {
      ev3OutSpeed(OUT_A, 100);
      ev3OutStart(OUT_A);
      state = value;
    } else if (value != state) {
      ev3OutStop(OUT_A, 1);
      state = value;
    }
    usleep(1000 * 100); // sleep for a 1/10th of a second
  }

  // Always, always, always free legoev3 when you are done
  ev3Free();
  return 0;
}
