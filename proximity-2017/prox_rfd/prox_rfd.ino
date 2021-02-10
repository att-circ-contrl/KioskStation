// Proximity sensor test program - RF Digital 77402
// Written by Christopher Thomas.

//
// Includes

// FIXME - Arduino library files have to go here as well, so that the IDE 
// knows to add them to the include/link paths.
#include <Wire.h>

// FIXME - Include SparkFun's library for the 77402.
#include <SparkFun_RFD77402_Arduino_Library.h>

// Primary project header file.
#include "prox_rfd_includes.h"


//
// Macros

#define STRINGBUFLEN 64


//
// Global Variables

char stringbuf[STRINGBUFLEN];



//
// Main Program


void setup(void)
{
  int idx;

  Serial.begin(115200);
  
  for (idx = 0; idx < STRINGBUFLEN; idx++)
    stringbuf[idx] = 0;

  I2C_Init();

  SensorInit();
  SensorStartMeasuring();
}


void loop(void)
{
  sensor_data_t data;
  bool valid;


  // If we don't have a sensor, look for one.
  // If we do have a sensor, take data from it.

  if (SensorIsPresent())
  {
    valid = SensorGetData(data);

    if (valid)
    {
      // Report this data.
      // (We might still have a sensor error condition.)

      snprintf(stringbuf, STRINGBUFLEN, "%4d mm   (amp %4d)  (%2d pix)",
        data.range, data.amplitude, data.pixels);
      stringbuf[STRINGBUFLEN-1] = 0;

      Serial.print(stringbuf);

      switch (data.errcode)
      {
        case SENCODE_VALID:  Serial.print("  [ok]\r\n"); break;
        case SENCODE_NEAR:  Serial.print("  [NEAR]\r\n"); break;
        case SENCODE_FAR:  Serial.print("  [FAR]\r\n"); break;
        default:  Serial.print("  [ERROR]\r\n"); break;
      }
    }
    else
    {
      // Report a problem.
      Serial.print("-- Invalid data. --\r\n");
    }

    // FIXME - Throttle sensor reporting. Shouldn't be needed.
    // The sensor's acquisition loop supposedly runs at 10 Hz.
//    delay(50);

    // FIXME - Throttle for debugging.
//    delay(1000);
  }
  else
  {
    // Report absence of a sensor.
    Serial.print("-- No sensor detected. --\r\n");

    // Throttle auto-probing.
    delay(1000);

    // Try probing again.
    SensorInit();
    if (SensorIsPresent())
      SensorStartMeasuring();
  }
}



//
// This is the end of the file.
