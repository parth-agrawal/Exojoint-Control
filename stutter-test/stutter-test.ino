#include <Servo.h>
#include <AS5X47.h>

Servo esc;
int throttlePin = 0;

// Define where the CSN Pin in connected.
int slaveSelectPin = 10;

// Start connection to the sensor.
AS5X47 as5047p(slaveSelectPin);
float lastRevCount = 0;
int revCount = 0;
float lastAngle = 0;
long lastTime = 0;
float startTime = millis();
long timeSample = 1;
float RPM = 0;
float rotPerSample = 0;

void setup()
{
  esc.attach(9);
  Serial.begin(1000000);
}

void loop()
{


  long currentTimeSec = (millis() - startTime) / 1000;
  
  if (currentTimeSec % 2 == 0) {
    esc.write(30);
    delay(500);
    esc.write(70);
  }


  esc.write(70);


  // interesting - throttle of 70 appears to be really close to 1000 per second



}
