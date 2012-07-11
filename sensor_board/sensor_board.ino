#include <Wire.h>
#include "FreqCounter.h" // Optional (not required by this code)
#include "HH10D2.h" 
#include "LowPower.h"
#include <SoftwareSerial.h>

using namespace std;
HH10D humid(384, 7688); //Using FreqCounter library
int count = 0;
#define BMP085_ADDRESS 0x77  // I2C address of BMP085
const unsigned char OSS = 0;  // Oversampling Setting
// Calibration values
int ac1;
int ac2;
int ac3;
unsigned int ac4;
unsigned int ac5;
unsigned int ac6;
int b1;
int b2;
int mb;
int mc;
int md;

// b5 is calculated in bmp085GetTemperature(...), this variable is also used in bmp085GetPressure(...)
// so ...Temperature(...) must be called before ...Pressure(...).
long b5;
float tempC;
float pressMB;
float rh;
int scount=0;
int tcount=0;
int sampling_secs=80;
int nwake_up=sampling_secs/8;

// These are for uLog
char buff[50];
int fileNumber = 1;
int statLED = 13;
int resetOpenLog = 4;

SoftwareSerial OpenLog(3, 2);

void setup(){
  //Serial.begin(9600);
  initialize_openLog();
  Wire.begin();
  bmp085Calibration();
  humid.setGateTime(500);
  wake_and_read();
  wake_and_read();
}

void loop()
{
  LowPower.powerSave(SLEEP_8S, ADC_OFF, BOD_OFF, TIMER2_OFF);
  if (tcount == 0) 
    {
      wake_and_read();
      log_sensor_values();
      //display_sensor_values();
    }
  tcount++;
  if (tcount > nwake_up) {tcount=0;}
}

void wake_and_read()
{
  readBMP085();
  readHH10D();
  
  //if (scount > 1) {display_sensor_values();}
 // if (scount < 2) { scount++;}
}


void display_sensor_values(void)
{
  Serial.println(nwake_up);
  Serial.print("Temperature : ");
  Serial.print(tempC);
  Serial.println(" C");
  
  Serial.print("Humidity : ");
  Serial.print(rh);
  Serial.println(" %");
  
  Serial.print("Pressure : ");
  Serial.print(pressMB);
  Serial.println(" mb");
  delay(500);
}

void log_sensor_values(void)
{
  OpenLog.println(nwake_up);
  OpenLog.print("Temperature : ");
  OpenLog.print(tempC);
  OpenLog.println(" C");
  
  OpenLog.print("Humidity : ");
  OpenLog.print(rh);
  OpenLog.println(" %");
  
  OpenLog.print("Pressure : ");
  OpenLog.print(pressMB);
  OpenLog.println(" mb");
  delay(500);
}
