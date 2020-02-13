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
int dcmFile = -1;
ANALOG* analog = MAP_FAILED;

// For some reason, the type of some devices connected to the ev3 is stored
// in the analog struct. IDK why though.

// Every time an input port is given by the user, subtract 16. (This is to ensure
// that there is no overlap between the input and output ports)

int8_t ev3InInit(void) {
  analogFile = open(ANALOG_DEVICE_NAME, O_RDWR);
  dcmFile = open(DCM_DEVICE_NAME, O_RDWR);
  if (analogFile < 0 && dcmFile < 0) {
    ev3InFree();
    return 0;
  }

  analog = mmap(NULL, sizeof(ANALOG), PROT_READ | PROT_WRITE, MAP_SHARED, analogFile, 0);
  close(analogFile); // Mappings persist after their files are closed
  analogFile = -1;
  if (analog == MAP_FAILED) {
    ev3InFree();
    return 0;
  }

  // Initialize the sensors
  int i = 0;
  TYPE type;
  CONN conn;
  char setup[4] = "----"; // dashes mean do nothing
  // I honestly don't know if this loop does what it is supposed to do.
  while (i < INPUTS) {
    type = ev3InGetType(i+16);
    conn = ev3InGetConn(i+16);
    if (type == TYPE_UNKNOWN && conn == CONN_INPUT_DUMB) { // for a touch sensor
      // Write a setup string
      setup[i] = 0x46; // this is the pin value from typedata.rcf for a touch sensor
      (*analog).InDcm[i] = type; // not sure if this does anything
    } else if (conn == CONN_INPUT_UART) {
      setup[i] = 0x22; // Setup pin value for a uart sensor
    }
    i++;
  }
  write(dcmFile, setup, 4);
  return 1; // It's the same as true
}

int16_t ev3InReadAnalogRaw(int8_t portNum) {
  // right shift to make the value for unpressed "fall off"
  portNum -= 16;
  return (*analog).Pin6[portNum][(*analog).LogIn[portNum]] >> 8;
}

// For some reason, device type data comes from d_analog.c
TYPE ev3InGetType(int8_t portNum) {
  if (portNum == IN_1 || portNum == IN_2 || portNum == IN_3 || portNum == IN_4) { 
    // It's an input port
    int8_t newPort = portNum - 16; // see line 28
    TYPE t = (TYPE)(*analog).InDcm[newPort];
    if ((ev3InGetConn(portNum) == CONN_INPUT_DUMB) && t == TYPE_UNKNOWN) {
      // touch sensor
      return TYPE_TOUCH;
    } else
      return t;
  } else if (portNum == OUT_A || portNum == OUT_B || portNum == OUT_C || portNum == OUT_D){
    portNum >>= 1; // This is for converting the args
    if (portNum & 0x04) portNum -= 1; // This makes 0x04 into 0x03
    return (TYPE)(*analog).OutDcm[portNum];
  } else {
    // user sucks
    return TYPE_ERROR;
  }
}

CONN ev3InGetConn(int8_t portNum) {
  if (portNum == IN_1 || portNum == IN_2 || portNum == IN_3 || portNum == IN_4) {
    portNum -= 16;
    return (CONN)(*analog).InConn[portNum];
  } else if (portNum == OUT_A || portNum == OUT_B || portNum == OUT_C || portNum == OUT_D) {
    portNum >>= 1; // This is for converting the args
    if (portNum & 0x04) portNum -= 1; // This makes 0x04 into 0x03
    return (CONN)(*analog).OutConn[portNum];
  } else 
    return CONN_ERROR; // user didn't specify a valid port
}

int8_t ev3InFree(void) {
  if (analog != MAP_FAILED) {
    munmap(analog, sizeof(ANALOG));
  }
  if (dcmFile >= 0) { // only close the file if it got opened
    close(dcmFile);
    dcmFile = -1;
  }
  return 1; // Equivalent to true
}