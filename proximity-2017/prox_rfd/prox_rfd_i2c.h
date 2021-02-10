// Proximity sensor test program - RF Digital 77402
// I2C routines.
// Written by Christopher Thomas.


//
// Functions


void I2C_Init(void);

uint8_t I2C_ReadReg8(uint8_t device, uint8_t regid);
uint16_t I2C_ReadReg16(uint8_t device, uint8_t regid);
void I2C_WriteReg8(uint8_t device, uint8_t regid, uint8_t data);
void I2C_WriteReg16(uint8_t device, uint8_t regid, uint16_t data);

// Error status persists until explicitly cleared. This allows checking a
// series of operations for problems without testing after every operation.
bool I2C_IsOk(void);
void I2C_ClearErrors(void);


//
// This is the end of the file.
