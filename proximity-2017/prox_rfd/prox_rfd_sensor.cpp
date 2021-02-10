// Proximity sensor test program - RF Digital 77402
// Sensor-specific routines.
// Written by Christopher Thomas.


//
// Includes

#include "prox_rfd_includes.h"


//
// Private Macros


// Select SparkFun's library if set.
#define USE_SPARKFUN_LIB 0


// Debugging.
#if 0
#define DPRINT(X); Serial.print(X);
#else
#define DPRINT(X); { /* Do nothing. */ }
#endif



//
// Private Constants

#define DEVID 0x4c

#define STATUSMASK 0x001f



//
// Private Variables

// Sensor detecton flag.
// This is updated whenever we perform an operation.
bool have_sensor = false;

#if USE_SPARKFUN_LIB
RFD77402 sensor;
#endif



//
// Private Functions


// This waits for the given status value, timing out after 100 ms.

bool PollForStatus(uint16_t code, uint16_t mask)
{
  bool found;
  int count;
  uint16_t data;

  found = false;

  // Initial quick-exit poll.
  data = I2C_ReadReg16(DEVID, 0x06);
  if ( I2C_IsOk() && (code == (data & mask)) )
    found = true;

  // The datasheet recommends polling 10 times over 100 ms.
  for (count = 0; I2C_IsOk() && (!found) && (10 > count); count++)
  {
    delay(10);

    data = I2C_ReadReg16(DEVID, 0x06);
    if ( I2C_IsOk() && (code == (data & mask)) )
      found = true;
  }

DPRINT("-- Status ");
DPRINT(code);
DPRINT(found ? " found.\r\n" : " not found!\r\n");
DPRINT(I2C_IsOk() ? "-- I2C ok.\r\n" : "-- I2C error!\r\n");

  return found;
}



// Try to set the appropriate MCPU state.
// Returns true on success, false, on timeout.


bool SetMCPUOff(void)
{
  bool result;

  I2C_WriteReg8(DEVID, 0x15, 0x05);
// FIXME - Try 16-bit version.
//I2C_WriteReg16(DEVID, 0x14, 0x0500);
  I2C_WriteReg8(DEVID, 0x04, 0x91);

  result = PollForStatus(0x10, STATUSMASK);

  return result;
}


bool SetMCPUOn(void)
{
  bool result;

  I2C_WriteReg8(DEVID, 0x15, 0x06);
// FIXME - Try 16-bit version.
//I2C_WriteReg16(DEVID, 0x14, 0x0600);
  I2C_WriteReg8(DEVID, 0x04, 0x92);

  // Datasheet flowchart and sample code disagree on this. Sample code is
  // more forgiving, so use its version.
  result = PollForStatus(0x18, STATUSMASK);

  return result;
}


bool SetMCPUStandby(void)
{
  bool result;

  I2C_WriteReg8(DEVID, 0x04, 0x90);

  result = PollForStatus(0x00, STATUSMASK);

  return result;
}



//
// Public Functions


#if USE_SPARKFUN_LIB


void SensorInit(void)
{
DPRINT("== Starting sensor detect/init.\r\n");

  // Defaults to Wire, 100kHz.
  have_sensor = sensor.begin();

DPRINT(have_sensor ? "== Found.\r\n" : "== Not found!\r\n");
}



void SensorStartMeasuring(void)
{
  // FIXME - SparkFun lib doesn't expose this separately.
}



void SensorStopMeasuring(void)
{
  // FIXME - SparkFun lib doesn't expose this separately.
}



bool SensorGetData(sensor_data_t &data)
{
  uint8_t code, result;

  data.range = 0x00;
  data.amplitude = 0x00;
  data.pixels = 0x00;
  data.errcode = SENCODE_ERROR;

  result = false;

  code = sensor.takeMeasurement();

  // No matter what error code we have, ask what the sensor object _thinks_
  // its data is.
  data.range = sensor.getDistance();
  data.amplitude = sensor.getConfidenceValue();
  data.pixels = sensor.getValidPixels();

  // Parse the return code.
  data.errcode = SENCODE_ERROR;
  switch (code)
  {
    case CODE_VALID_DATA : data.errcode = SENCODE_VALID; break;
    case CODE_FAILED_PIXELS : data.errcode = SENCODE_NEAR; break;
    case CODE_FAILED_SIGNAL : data.errcode = SENCODE_FAR; break;

    // NOTE - Special-case the timeout, as that indicates device detection
    // failure.
    case CODE_FAILED_TIMEOUT : have_sensor = false;

    default : break;
  }

  if (SENCODE_VALID == data.errcode)
    result = true;

  return result;
}



// This will be false if we haven't probed for the sensor.
// It will also be false if past probe attempts failed.

bool SensorIsPresent(void)
{
  return have_sensor;
}


#else


void SensorInit(void)
{
  bool isok, result;
  uint16_t data;

  // Clear any accumulated errors.
  I2C_ClearErrors();

DPRINT("== Starting sensor detect/init.\r\n");

  // Power-on sequence from the datasheet.
  // Cross-checked against sample code.

  // Wait a moment for power to settle.
  delay(100);

#if 0
// FIXME - Read chipid. We shouldn't _have_ to.
data = I2C_ReadReg16(DEVID, 0x28);
DPRINT("-- Reported chipid:  ");
DPRINT(data);
DPRINT("\r\n");
#endif

  // Check standby.
//  isok = PollForStatus(0x00, STATUSMASK);
// FIXME - Explicitly force standby, per sample code.
isok = SetMCPUStandby();

  // Set up interrupt pad.
  I2C_WriteReg8(DEVID, 0x00, 0x04);

  // Set up I2C.
  I2C_WriteReg8(DEVID, 0x1c, 0x65);

  // Reboot the MCPU.
  result = SetMCPUOff();
  isok = isok && result;
  result = SetMCPUOn();
  isok = isok && result;

  // Configure time-of-flight information.
  // Datasheet doesn't describe what these do; they're black-box.
  // The exception is 0x26 bit 7 ( 1 = auto-adjust integration time ).
  I2C_WriteReg16(DEVID, 0x0c, 0xe100);
  I2C_WriteReg16(DEVID, 0x0e, 0x10ff);
  I2C_WriteReg16(DEVID, 0x20, 0x07d0);
  I2C_WriteReg16(DEVID, 0x22, 0x5008);
  I2C_WriteReg16(DEVID, 0x24, 0xa041);
  I2C_WriteReg16(DEVID, 0x26, 0x45d4);

  // Go to standby mode.
  result = SetMCPUStandby();
  isok = isok && result;


  // If an I2C error occurred, flag the problem.
  result = I2C_IsOk();
  isok = isok && result;


DPRINT(result ? "-- I2C ok.\r\n" : "-- I2C error!\r\n");
DPRINT(isok ? "== Found.\r\n" : "== Not found!\r\n");

  // Update our "found" flag.
  have_sensor = isok;
}


// Measuring sequence components from the datasheet.


void SensorStartMeasuring(void)
{
  bool isok, result;

  if (have_sensor)
  {
    isok = true;

    // Reboot the MCPU.
    result = SetMCPUOff();
    isok = isok && result;
    result = SetMCPUOn();
    isok = isok && result;

    // Check for I2C errors.
    result = I2C_IsOk();
    isok = isok && result;

    // Update our "found" flag.
    have_sensor = isok;
  }
}


void SensorStopMeasuring(void)
{
  bool isok, result;

  if (have_sensor)
  {
    isok = true;

    // Go to standby mode.
    result = SetMCPUStandby();
    isok = isok && result;

    // Check for I2C errors.
    result = I2C_IsOk();
    isok = isok && result;

    // Update our "found" flag.
    have_sensor = isok;
  }
}


// Returns true if valid data was present.
// False might just be due to polling too quickly?

bool SensorGetData(sensor_data_t &data)
{
  bool data_valid;
  bool ready;
  int count;
  uint8_t status;
  uint16_t rawdistance, rawamplitude;

  data_valid = false;

  if (have_sensor)
  {
    // Trigger a measurement.
    I2C_WriteReg8(DEVID, 0x04, 0x81);


    // Spin on "data ready", timing out after 100 ms.

    ready = false;

    // Initial quick-exit poll.
    status = I2C_ReadReg8(DEVID, 0x00);
    if (status & 0x10)
      ready = true;

    // The datasheet recommends polling 10 times over 100 ms.
    for (count = 0; I2C_IsOk() && (!ready) && (10 > count); count++)
    {
      delay(10);

      status = I2C_ReadReg8(DEVID, 0x00);
      if (status & 0x10)
        ready = true;
    }

    // Proceed only if we have data.
    if (ready)
    {
      rawdistance = I2C_ReadReg16(DEVID, 0x08);
      rawamplitude = I2C_ReadReg16(DEVID, 0x0a);

      data.range = 0x00;
      data.amplitude = 0x00;
      data.pixels = 0x00;
      data.errcode = SENCODE_ERROR;

      if (I2C_IsOk())
      {
        // We have _something_.
        data_valid = true;

        // This is 12 bits in the flowchart, but is actually 11 bits.
        data.range = (rawdistance >> 2) & 0x07ff;

        rawdistance >>= 13;
        rawdistance &= 2;
        switch (rawdistance)
        {
          case 0b00 : data.errcode = SENCODE_VALID; break;
          case 0b01 : data.errcode = SENCODE_NEAR; break;
          case 0b10 : data.errcode = SENCODE_FAR; break;
          default : data.errcode = SENCODE_ERROR; break;
        }

        // In this case, "far" means "far".
        // If we got it in the previous step, it means "return too weak".
        // FIXME - Just leave this reporting the maximum value.
//        if (0x07ff == data.range)
//          data.errcode = SENCODE_FAR;

        // "Amplitude" is called "confidence" in the sample code?
        data.amplitude = (rawamplitude >> 4) & 0x07ff;

        data.pixels = rawamplitude & 0x000f;
      }
    }


    // Take note of any I2C errors.
    have_sensor = I2C_IsOk();
  }

  return data_valid;
}



// This will be false if we haven't probed for the sensor.
// This will also be false if any operation has I2C bus errors.

bool SensorIsPresent(void)
{
  // Doublecheck I2C error state.
  if (!( I2C_IsOk() ))
    have_sensor = false;

  return have_sensor;
}


#endif


//
// This is the end of the file.
