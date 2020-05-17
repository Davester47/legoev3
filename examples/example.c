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

#include <ev3.h>
#include <unistd.h>

int main() {
// Always initialise legoev3
  ev3Init();

  // Play a tone at 500 Hz for half of a second
  ev3SndTone(500, 500, 100); // Does not block.
  // If you want to wait until the end of a sound, use this:
  ev3SndReady(-1);

  // Turn the blinking LEDs orange if the back button is pressed
  if (ev3BtnRead(BACK_BUTTON)) {
    ev3SetLEDPattern(LED_ORANGE_FLASH);
  }

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

  // Read from the color sensor on port 2
  ev3InSetMode(IN_2, 2);
  sleep(1); // If you don't wait a little bit after changing modes, you get garbage
  int32_t color = ev3InRead(IN_2);

  // Always, always, always free legoev3 when you are done
  ev3Free();
  return 0;
}
