/*
 * libev3 - a simple library for Lego Mindstorms
 * Copyright (C) 2020 David Stumph
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
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


#define EV3_C
#include <stddef.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include "lms2012.h"
#include "lmstypes.h"
#include "bytecodes.h"
#include "c_output.h"
#include "ev3.h"

void ev3Log(const char* message, ...) {
  va_list vl;
  va_start(vl, message);
  FILE* pFile = fopen("/home/root/lms2012/prjs/test/log", "a");
  vfprintf(pFile, message, vl);
  fclose(pFile);
  va_end(vl);
}

int ev3Init() {
  cOutputInit(); // A built-in function on the EV3
  return 1;
}


// Parameter Encoding variables
#define EV3_MAX_PARAMS 7
void* params[EV3_MAX_PARAMS]; // Staging area for parameters
int currentParam = 0;
char daisy = 0;

// Call this before calling a firmware function.
void ev3ClearParams() {
  currentParam = 0;
  while (currentParam < EV3_MAX_PARAMS) {
    params[currentParam] = NULL;
    currentParam++;
  }
  currentParam = 0;
  params[0] = &daisy; // telling the firmware to not use daisychaining
}

// Interface for firmware to retrieve parameters passed to functions. The
// firmware expects this function to be defined.
void* PrimParPointer() {
  ev3Log("Passing param\n");
  return params[currentParam++];
}

// Other functions expected by the firmware
IP GetObjectIp(void) {
  IP ip = NULL;
  ev3Log("GetObjectIp was called\n");
  return ip;
}
extern    void      SetDispatchStatus(DSPSTAT Status) {
  // do nothing (we don't care)
  return;
}

extern    void      LogErrorNumber(ERR Error) {
  ev3Log("Error occured!\n");
  exit(1);
  return;
};

extern    OBJID     CallingObjectId(void) {
  // Make something up, since I'm pretty sure it doesn't matter.
  // (might be a little bit wrong, though)
  return 255;
}

void SetObjectIp(IP ip) {
  return;
}

UBYTE cDaisyCheckBusyBit (DATA8 Layer, DATA8 Nos) {
  return 0;
};

////////////////////////////////////////////////////////////////////////////////
// Output functions
////////////////////////////////////////////////////////////////////////////////

void ev3OutSetType (char portNum, TYPE deviceType) {
  ev3ClearParams();
  params[1] = (void*)&portNum;
  signed char devType = (signed char)deviceType;
  params[2] = (void*)&devType;
  cOutputSetType();
}

void ev3OutStart(char ports) {
  ev3ClearParams();
  params[1] = &ports;
  cOutputStart();
}

void ev3OutStop(char ports, char brake) {
  ev3ClearParams();
  params[1] = &ports;
  params[2] = &brake;
  cOutputStop();
}

void ev3OutSpeed(char ports, char speed) {
  ev3ClearParams();
  params[1] = &ports;
  params[2] = &speed;
  cOutputSpeed();
}

void ev3OutPower(char ports, char power) {
  ev3ClearParams();
  params[1] = &ports;
  params[2] = &power;
  cOutputPower();
}

void ev3OutReset(char ports) {
  ev3ClearParams();
  params[1] = &ports;
  cOutputReset();
}

void ev3OutPolarity(char ports, char pol) {
  ev3ClearParams();
  params[1] = &ports;
  params[2] = &pol;
  cOutputPolarity();
}

void ev3OutTimePower(char ports, char power, int time1, int time2, int time3,
                     char brake) {
  ev3ClearParams();
  params[1] = &ports;
  params[2] = &power;
  params[3] = &time1;
  params[4] = &time2;
  params[5] = &time3;
  params[6] = &brake;
  cOutputTimePower();
}

/*
// Currently broken: returns 124 speed and 0 steps or some random garbage
void ev3OutRead(char portNum, char* speed, int* steps) {
  ev3ClearParams();
  params[1] = &portNum;
  params[2] = speed;
  params[3] = steps;
  cOutputRead();
}
*/
void ev3OutTimeSpeed(char ports, char speed, int time1, int time2, int time3,
                     char brake) {
  ev3ClearParams();
  params[1] = &ports;
  params[2] = &speed;
  params[3] = &time1;
  params[4] = &time2;
  params[5] = &time3;
  params[6] = &brake;
  cOutputTimeSpeed(); // A built-in function on the EV3
}

void ev3OutStepPower(char ports, char power, int step1, int step2, int step3,
                     char brake) {
  ev3ClearParams();
  params[1] = &ports;
  params[2] = &power;
  params[3] = &step1;
  params[4] = &step2;
  params[5] = &step3;
  params[6] = &brake;
  cOutputStepPower(); // A built-in function on the EV3
}

void ev3OutStepSpeed(char ports, char speed, int step1, int step2, int step3,
                     char brake) {
  ev3ClearParams();
  params[1] = &ports;
  params[2] = &speed;
  params[3] = &step1;
  params[4] = &step2;
  params[5] = &step3;
  params[6] = &brake;
  cOutputStepSpeed(); // A built-in function on the EV3
}

void ev3OutStepSync(char ports, char speed, signed short turn, int step, char brake) {
  ev3ClearParams();
  params[1] = &ports;
  params[2] = &speed;
  params[3] = &turn;
  params[4] = &step;
  params[5] = &brake;
  cOutputStepSync();
}

void ev3OutTimeSync(char ports, char speed, signed short turn, int time, char brake) {
  ev3ClearParams();
  params[1] = &ports;
  params[2] = &speed;
  params[3] = &turn;
  params[4] = &time;
  params[5] = &brake;
  cOutputTimeSync();
}

int ev3Free() {
  cOutputExit();
  return 1;
}
