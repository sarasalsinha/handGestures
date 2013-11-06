/*
 
 This sketch uses a Triple Axis Accelerometer Breakout (MMA8452Q), from Sparkfun.
 Reads X/Y/Z and other basic functions of this accelerometer. Please see the Sparkfun example code at: https://github.com/sparkfun/MMA8452_Accelerometer
 
 I'm using an Arduino Uno with this configuration:
 
 Accelerometer ------------ Arduino
 ----------------------------------
 3.3V --------------------- 3.3V
 SDA ---------------------- A4
 SCL ---------------------- A5
 GND ---------------------- GND
 
 */

#include <Wire.h>

#define MMA8452_ADDRESS 0x1D
#define OUT_X_MSB 0x01
#define XYZ_DATA_CFG  0x0E
#define WHO_AM_I   0x0D
#define CTRL_REG1  0x2A
#define GSCALE 2 // +/-2, 4, or 8g


void setup()
{
  Serial.begin(9600);
  //Serial.println("Triple Axis Accelerometer Breakout (MMA8452Q) - XYZ Simple Example");

  Wire.begin();
  initMMA8452();
}


void loop()
{  
  int accelCount[3];
  readAccelData(accelCount);

  // To calculate the accleration value into g's
  float accelG[3];
  for (int i = 0 ; i < 3 ; i++)
  {
    accelG[i] = (float) accelCount[i] / ((1<<12)/(2*GSCALE));
  }

// 'nDecimal' number of number after the "0."
  int nDecimal = 4;
  if ( Serial.available() > 0 ) 
  {
    byte inbyte = Serial.read();
    // if it gets an 'a' from processing, prints the line
    if(inbyte == 'a'){
      Serial.print(accelG[0], nDecimal);
      Serial.print(",");
      Serial.print(accelG[1], nDecimal);
      Serial.print(",");
      Serial.print(accelG[2], nDecimal);
      Serial.println();
    } 
  }

  delay(1);
}


void readAccelData(int *destination)
{
  byte rawData[6];

  readRegisters(OUT_X_MSB, 6, rawData);

  for(int i = 0; i < 3 ; i++)
  {
    int gCount = (rawData[i*2] << 8) | rawData[(i*2)+1];
    gCount >>= 4;

    if (rawData[i*2] > 0x7F)
    {  
      gCount = ~gCount + 1;
      gCount *= -1;
    }

    destination[i] = gCount;
  }
}


void initMMA8452()
{
  byte c = readRegister(WHO_AM_I);
  if (c == 0x2A)
  {  
    //Serial.println("MMA8452Q is connected.");
  }
  else
  {
    //Serial.print("Could not connect to MMA8452Q: 0x");
    //Serial.println(c, HEX);
    while(1) ;
  }

  MMA8452Standby();

  byte fsr = GSCALE;
  if(fsr > 8) fsr = 8;
  fsr >>= 2;
  writeRegister(XYZ_DATA_CFG, fsr);

  MMA8452Active();
}

void MMA8452Standby()
{
  byte c = readRegister(CTRL_REG1);
  writeRegister(CTRL_REG1, c & ~(0x01));
}

void MMA8452Active()
{
  byte c = readRegister(CTRL_REG1);
  writeRegister(CTRL_REG1, c | 0x01);
}


void readRegisters(byte addressToRead, int bytesToRead, byte * dest)
{
  Wire.beginTransmission(MMA8452_ADDRESS);
  Wire.write(addressToRead);
  Wire.endTransmission(false);

  Wire.requestFrom(MMA8452_ADDRESS, bytesToRead);

  while(Wire.available() < bytesToRead);

  for(int x = 0 ; x < bytesToRead ; x++)
    dest[x] = Wire.read();    
}

byte readRegister(byte addressToRead)
{
  Wire.beginTransmission(MMA8452_ADDRESS);
  Wire.write(addressToRead);
  Wire.endTransmission(false);

  Wire.requestFrom(MMA8452_ADDRESS, 1);

  while(!Wire.available()) ;
  return Wire.read();
}

void writeRegister(byte addressToWrite, byte dataToWrite)
{
  Wire.beginTransmission(MMA8452_ADDRESS);
  Wire.write(addressToWrite);
  Wire.write(dataToWrite);
  Wire.endTransmission();
}

