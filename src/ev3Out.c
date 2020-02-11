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

#include "ev3Private.h"
#include <time.h>

MOTORDATA* motordata; // 4 for 4 outputs
int motorFile = -1;
int pwmFile = -1;


int8_t ev3OutInit (void) {
  // initialize the kernel driver interface
  motorFile = open(MOTOR_DEVICE_NAME, O_RDWR);
  pwmFile = open(PWM_DEVICE_NAME, O_RDWR);

  if (!(motorFile >= 0 && pwmFile >= 0)) {
    // something went wrong
    ev3Log("Error opening files!\n");
    return 0;
  }
  // Map the MOTORDATA structure
  motordata = (MOTORDATA*) mmap(NULL, sizeof(MOTORDATA) * OUTPUTS, PROT_READ | PROT_WRITE,
               MAP_FILE | MAP_SHARED, motorFile, 0);
  if (motordata == MAP_FAILED) {
    // Something went wrong
    ev3Log("Error mapping %s\n", MOTOR_DEVICE_NAME);
    return 0;
  }
  // Send the program start opcode
  char programStart = opPROGRAM_START;
  write(pwmFile, &programStart, 1);
  return 1;
}

void ev3OutStart(char ports) {
  char args[2] = {opOUTPUT_START, ports};
  write(pwmFile, args, 2);
}

void ev3OutSpeed(char ports, char speed) {
  char args[3] = {opOUTPUT_SPEED, ports, speed};
  write(pwmFile, args, 3);
}

void ev3OutStop(char ports, char brake) {
  char args[3] = {opOUTPUT_STOP, ports, brake};
  write(pwmFile, args, 3);
}

void ev3OutReset(char ports) {
  char args[2] = {opOUTPUT_RESET, ports};
  write(pwmFile, args, 2);
}

void ev3OutPower(char ports, char power) {
  char args[3] = {opOUTPUT_POWER, ports, power};
  write(pwmFile, args, 3);
}

// Changes the direction of all motor speed parameters. 1: normal, 0: toggle
// -1: reverse.
void ev3OutPolarity(char ports, char pol) {
  char args[3] = {opOUTPUT_POLARITY, ports, pol};
  write(pwmFile, args, 3);
}

void ev3OutStepPower(char ports, char power, int step1, int step2, int step3,
  char brake) {
  STEPPOWER arg = {opOUTPUT_STEP_POWER, ports, power, step1, step2, step3, brake};
  write(pwmFile, &arg, sizeof(arg));
}
void ev3OutTimePower(char ports, char power, int time1, int time2, int time3, char brake) {
  TIMEPOWER arg = {opOUTPUT_TIME_POWER, ports, power, time1, time2, time3, brake};
  write(pwmFile, &arg, sizeof(arg));
}
void ev3OutStepSpeed(char ports, char speed, int step1, int step2, int step3, char brake) {
  STEPSPEED arg = {opOUTPUT_STEP_SPEED, ports, speed, step1, step2, step3, brake};
  write(pwmFile, &arg, sizeof(arg));
}
void ev3OutTimeSpeed(char ports, char speed, int time1, int time2, int time3, char brake) {
  TIMESPEED arg = {opOUTPUT_TIME_SPEED, ports, speed, time1, time2, time3, brake};
  write(pwmFile, &arg, sizeof(arg));
}

char checkPorts(char ports, char numPorts) {
  char i = 0;
  char num = 0;
  while (i < OUTPUTS) {
    if ((ports >> i) % 2) { // check all the output bits
      num++;
    }
    i++;
  }
  if (num == numPorts && ports < 16) return true;
  else return false;
}
// Ports CAN ONLY CONTAIN 2 OUTPUTS! Expect these functions not to work if you
// give it less or more than two ports.
void ev3OutStepSync(char ports, char speed, signed short turn, int step, char brake) {
  if (!checkPorts(ports, 2)) return;
  STEPSYNC arg = {opOUTPUT_STEP_SYNC, ports, speed, turn, step, brake};
  write(pwmFile, &arg, sizeof(arg));
}
void ev3OutTimeSync(char ports, char speed, signed short turn, int time, char brake) {
  if (!checkPorts(ports, 2)) return;
  TIMESYNC arg = {opOUTPUT_TIME_SYNC, ports, speed, turn, time, brake};
  write(pwmFile, &arg, sizeof(arg));
};

// Waits until the motor is done.
// If maxWait = -1, it could wait forever, otherwise it could
// wait maxWait seconds. If maxWait = 0, it will return the busy state.
bool ev3OutReady(char ports, int maxWait) {
  unsigned char buffer[4];
  time_t startTime = time(NULL);
  bool retval = true;
  do {
    read(pwmFile, buffer, 4);
    if (!(buffer[0] & ports)) {
      retval = true;
      break;
    }
    retval = false;
    usleep(1000); // Sleep for a millisecond and not a resource hog
  } while ((difftime(time(NULL), startTime) <= maxWait || maxWait == 0) && !(maxWait < 0));
  return retval;
}

void ev3OutRead(char portNum, char* speed, int* steps) {
  if (!checkPorts(portNum, 1)) return;
  portNum >>= 1; // This is for converting the args
  if (portNum & 0x04) portNum -= 1; // This makes 0x04 into 0x03
  *speed = motordata[portNum].Speed;// Read out the value from shared memory
  *steps = motordata[portNum].TachoCounts;
}

void ev3OutClrCount(char ports) {
  char args[2] = {opOUTPUT_CLR_COUNT, ports};
  write(pwmFile, args, 2);
}

void ev3OutSetAllTypes(TYPE devType[]) {
  char args[5] = {opOUTPUT_SET_TYPE,devType[0],devType[1],devType[2],devType[3]};
  write(pwmFile, args, 5);
}

int8_t ev3OutFree (void) {
  // Get rid of everything
  // Only close the files IF they were opened earlier.
  if (motorFile >= 0) {
    if (motordata != MAP_FAILED) {
      munmap(motordata, sizeof(MOTORDATA) * OUTPUTS);
    }
    close(motorFile);
  }
  if (pwmFile >= 0) {
    char arg = opPROGRAM_STOP;
    write(pwmFile, &arg, 1);
    close(pwmFile);
  }
  return 1;
}
