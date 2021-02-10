// Proximity sensor test program - Sharp 2Y0A21YK0F
// Written by Christopher Thomas.

//
// Includes


//
// Macros

#define STRINGBUFLEN 16


//
// Global Variables

char stringbuf[STRINGBUFLEN];



//
// Main Program


void setup(void)
{
  int idx;
  
  analogReference(INTERNAL2V56);

  Serial.begin(115200);
  
  for (idx = 0; idx < STRINGBUFLEN; idx++)
    stringbuf[idx] = 0;
}


void loop(void)
{
  int thisval;


  // Acquire and report a sample.
  // FIXME - Arduino's version of xxprintf does not handle floats, for code
  // complexity/space reasons.

  thisval = analogRead(0);

  // FIXME - Fake 3-decimal-place floating point output.
  // 2.56 V / 1024 = 2.5 mV.
  thisval *= 25;  // 0..25600.
  thisval /= 10;  // Convert to mV, rounding down.
  
  snprintf(stringbuf, STRINGBUFLEN, "%d.%03d V\r\n",
    thisval / 1000, thisval % 1000);
  stringbuf[STRINGBUFLEN-1] = 0;

  Serial.print(stringbuf);


  // Reduce to a reasonable readout rate.
  delay(50);
}



//
// This is the end of the file.
