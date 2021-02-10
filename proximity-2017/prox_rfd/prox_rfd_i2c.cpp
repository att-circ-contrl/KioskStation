// Proximity sensor test program - RF Digital 77402
// I2C routines.
// Written by Christopher Thomas.


//
// Includes

#include "prox_rfd_includes.h"



//
// Private Macros


// Set this to stub out error checks, for debugging.
#define IGNORE_ERRORS 0

#if IGNORE_ERRORS
#define ERRWRAP(X) /* Do nothing */
#else
#define ERRWRAP(X) X
#endif


// Set this to enable I2C tattling.
// NOTE - This will slow down I2C! Reports take a while!
#define TATTLE_I2C 0

#if TATTLE_I2C
#define DPRINT(X); Serial.print(X);
#else
#define DPRINT(X); { /* Do nothing. */ }
#endif



//
// Private Variables

// Status/error flag.
bool i2c_ok;



//
// Functions


void I2C_Init(void)
{
  Wire.begin();
  I2C_ClearErrors();
}



uint8_t I2C_ReadReg8(uint8_t device, uint8_t regid)
{
  uint8_t result;
  int bytes_written, bytes_read;

  result = 0x00;

  // Perform the whole transaction even if a failure occurs.

  Wire.beginTransmission(device);
  bytes_written = Wire.write(regid);
  Wire.endTransmission();

  Wire.requestFrom(device, (uint8_t) 1);
  bytes_read = Wire.available();

ERRWRAP(  if (1 == bytes_read)  )
  {
    result = Wire.read();
  }

  if ( (1 != bytes_written) || (1 != bytes_read) )
    i2c_ok = false;

  DPRINT("..8-bit read:  ");
  DPRINT(regid);
  DPRINT(" -> ");
  DPRINT(result);
  DPRINT("\r\n");

  return result;
}



uint16_t I2C_ReadReg16(uint8_t device, uint8_t regid)
{
  uint16_t result;
  int bytes_written, bytes_read;
  uint16_t msb, lsb;

  result = 0x0000;

  // Perform the whole transaction even if a failure occurs.

  Wire.beginTransmission(device);
  bytes_written = Wire.write(regid);
  Wire.endTransmission();

  Wire.requestFrom(device, (uint8_t) 2);
  bytes_read = Wire.available();

ERRWRAP(  if (2 == bytes_read) )
  {
    lsb = Wire.read();
    msb = Wire.read();

    result = (msb << 8) | lsb;
  }

  if ( (1 != bytes_written) || (2 != bytes_read) )
    i2c_ok = false;

  DPRINT("..16-bit read:  ");
  DPRINT(regid);
  DPRINT(" -> ");
  DPRINT(result);
  DPRINT("\r\n");

  return result;
}



void I2C_WriteReg8(uint8_t device, uint8_t regid, uint8_t data)
{
  int bytes_written;

  Wire.beginTransmission(device);

  // Send the whole message even if a failure occurs.
  bytes_written = 0;
  bytes_written += Wire.write(regid);
  bytes_written += Wire.write(data);

  Wire.endTransmission();

  if (2 != bytes_written)
    i2c_ok = false;

  DPRINT("..8-bit write:  ");
  DPRINT(regid);
  DPRINT(" <- ");
  DPRINT(data);
  DPRINT("\r\n");
}



void I2C_WriteReg16(uint8_t device, uint8_t regid, uint16_t data)
{
  int bytes_written;
  uint16_t msb, lsb;

  msb = (data >> 8) & 0xff;
  lsb = data & 0xff;

  Wire.beginTransmission(device);

  // Send the whole message even if a failure occurs.
  bytes_written = 0;
  bytes_written += Wire.write(regid);
  bytes_written += Wire.write(lsb);
  bytes_written += Wire.write(msb);

  Wire.endTransmission();

  if (3 != bytes_written)
    i2c_ok = false;

  DPRINT("..16-bit write:  ");
  DPRINT(regid);
  DPRINT(" <- ");
  DPRINT(data);
  DPRINT("\r\n");
}



// Error status persists until explicitly cleared. This allows checking a
// series of operations for problems without testing after every operation.

bool I2C_IsOk(void)
{
  bool result;

  result = true;
ERRWRAP(  result &= i2c_ok;  )

  return result;
}


void I2C_ClearErrors(void)
{
  i2c_ok = true;
}



//
// This is the end of the file.
