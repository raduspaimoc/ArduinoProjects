/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010,
                 2011,2012 Giovanni Di Sirio.

    This file is part of ChibiOS/RT.

    ChibiOS/RT is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    ChibiOS/RT is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "ch.h"
#include "hal.h"
#include "chprintf.h"

static const uint8_t slave_address = 0x30;

struct CoordsData
{
  int x;
  int y;
  int z;
};

uint8_t distance = 0;
uint8_t humidity = 0;
uint8_t temperature = 0;

BSEMAPHORE_DECL(smph,2);
struct CoordsData coords;

uint8_t request[]={0,0,0,0,0};
uint8_t result[]={0,0,0,0,0,0};

static WORKING_AREA(waThread_I2C, 128);
static msg_t Thread_I2C(void *p) {
  (void)p;
  chRegSetThreadName("SerialPrintI2C");
  msg_t status;

  // Some time to allow slaves initialization
  chThdSleepMilliseconds(2000);

  while (TRUE) {

    // Request values
    request[0]=0x31;

    chBSemWait(&smph); // Using I2C bus
    i2cMasterTransmit( &I2C0, 0x53, request, 1, result, 6);
    chBSemSignal(&smph);

    coords.x = (((int)result[1]) << 8) | result[0];
    coords.y = (((int)result[3]) << 8) | result[2];
    coords.z = (((int)result[5]) << 8) | result[4];

    chThdSleepMilliseconds(300);
  }
  return 0;
}

static WORKING_AREA(waThread_LCD, 128);
static msg_t Thread_LCD(void *p) {
  (void)p;
  chRegSetThreadName("ThreadLCD");
  msg_t status;

  // Some time to allow slaves initialization
  chThdSleepMilliseconds(3000);

  while (TRUE) {

    sdPut(&SD1, (uint8_t)0x7C);
    sdPut(&SD1, (uint8_t)0x00);

    chThdSleepMilliseconds(10);

    sdPut(&SD1, (uint8_t)0x7C);
    sdPut(&SD1, (uint8_t)0x18);
    sdPut(&SD1, (uint8_t)0x00);
    chThdSleepMilliseconds(10);

    sdPut(&SD1, (uint8_t)0x7C);
    sdPut(&SD1, (uint8_t)0x19);
    sdPut(&SD1, (uint8_t)0x35);
    chThdSleepMilliseconds(10);

    chBSemWait(&smph); // Semaphore to not read inconsistent data
    chprintf((BaseSequentialStream *)&SD1, "X: %05d Y: %05d Z: %05d", coords.x, coords.y, coords.z);

    sdPut(&SD1, (uint8_t)0x7C);
    sdPut(&SD1, (uint8_t)0x18);
    sdPut(&SD1, (uint8_t)0x00);
    chThdSleepMilliseconds(10);

    sdPut(&SD1, (uint8_t)0x7C);
    sdPut(&SD1, (uint8_t)0x19);
    sdPut(&SD1, (uint8_t)0x35 - 16);
    chThdSleepMilliseconds(10);

    chprintf((BaseSequentialStream *)&SD1, "Distance: %d", distance);
    sdPut(&SD1, (uint8_t)0x7C);
    sdPut(&SD1, (uint8_t)0x18);
    sdPut(&SD1, (uint8_t)0x00);
    chThdSleepMilliseconds(10);

    sdPut(&SD1, (uint8_t)0x7C);
    sdPut(&SD1, (uint8_t)0x19);
    sdPut(&SD1, (uint8_t)0x35 - 24);
    chThdSleepMilliseconds(10);

    chprintf((BaseSequentialStream *)&SD1, "Temperature: %d", temperature);
    sdPut(&SD1, (uint8_t)0x7C);
    sdPut(&SD1, (uint8_t)0x18);
    sdPut(&SD1, (uint8_t)0x00);

    chThdSleepMilliseconds(10);
    sdPut(&SD1, (uint8_t)0x7C);
    sdPut(&SD1, (uint8_t)0x19);
    sdPut(&SD1, (uint8_t)0x35 - 32);
    chThdSleepMilliseconds(10);
    chprintf((BaseSequentialStream *)&SD1, "Humidity: %d", humidity);
    chBSemSignal(&smph);

    chThdSleepMilliseconds(300);
    chThdSleepMilliseconds(5000);
  }
  return 0;
}

static WORKING_AREA(waThread_Arduino, 128);
static msg_t Thread_Arduino(void *p) {
  (void)p;
  chRegSetThreadName("ThreadArduino");
  msg_t status;

  // Some time to allow slaves initialization
  chThdSleepMilliseconds(100);

  while (TRUE) {

    // Request values
    uint8_t request = 1;
    uint8_t result;
    chBSemWait(&smph);
    i2cMasterTransmit( &I2C0, 0x30, &request, 1, &temperature, 1);
    chBSemSignal(&smph);
    chThdSleepMilliseconds(500);

    request = 2;
    chBSemWait(&smph);
    i2cMasterTransmit( &I2C0, 0x30, &request, 1, &humidity, 1);
    chBSemSignal(&smph);
    chThdSleepMilliseconds(500);

    request = 3;
    chBSemWait(&smph);
    i2cMasterTransmit( &I2C0, 0x30, &request, 1, &distance, 1);
    chBSemSignal(&smph);
    chThdSleepMilliseconds(500);
  }
  return 0;
}

int main(void) {
  halInit();
  chSysInit();

  chBSemInit(&smph, 0);

  // Initialize Serial Port
  sdStart(&SD1, NULL);

  /*
   * I2C initialization.
   */
  I2CConfig i2cConfig;
  i2cStart(&I2C0, &i2cConfig);
  request[0]=0x2D;
  request[1]=0x00;
  i2cMasterTransmit( &I2C0, 0x53, request, 2, result, 0);

  request[1]=0x10;
  i2cMasterTransmit( &I2C0, 0x53, request, 2, result, 0);
  chThdSleepMilliseconds(10);

  request[1]=0x08;
  i2cMasterTransmit( &I2C0, 0x53, request, 2, result, 0);
  chThdSleepMilliseconds(10);


  chThdCreateStatic(waThread_I2C, sizeof(waThread_I2C), HIGHPRIO, Thread_I2C, NULL);
  chThdCreateStatic(waThread_Arduino, sizeof(waThread_Arduino), HIGHPRIO, Thread_Arduino, NULL);
  chThdCreateStatic(waThread_LCD, sizeof(waThread_LCD), HIGHPRIO, Thread_LCD, NULL);

  // Blocks until finish
  chThdWait(chThdSelf());

  return 0;
}
