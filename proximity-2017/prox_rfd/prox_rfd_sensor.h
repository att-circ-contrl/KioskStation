// Proximity sensor test program - RF Digital 77402
// Sensor-specific routines.
// Written by Christopher Thomas.


//
// Typedefs and Enums

typedef enum
{
  SENCODE_VALID = 0b00,
  SENCODE_NEAR = 0b01,
  SENCODE_FAR = 0b10,
  SENCODE_ERROR = 0b11
} sensor_errcode_t;


typedef struct
{
  uint16_t range;
  uint16_t amplitude;
  uint8_t pixels;
  sensor_errcode_t errcode;
} sensor_data_t;


//
// Functions


void SensorInit(void);
void SensorStartMeasuring(void);
void SensorStopMeasuring(void);

// Returns true if valid data was present.
// False might just be due to polling too quickly?
bool SensorGetData(sensor_data_t &data);

// This will be false if we haven't probed for the sensor.
// This will also be false if any operation has I2C bus errors.
bool SensorIsPresent(void);


//
// This is the end of the file.
