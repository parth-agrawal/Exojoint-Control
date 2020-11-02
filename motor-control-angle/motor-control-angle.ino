// THIS IS THE GOOD ONE

#include <Servo.h>
#include <AS5X47.h>

Servo esc;
int throttlePin = 0;

// Define where the CSN Pin in connected.
int slaveSelectPin = 10;

// Start connection to the sensor.
AS5X47 as5047p(slaveSelectPin);
float lastRevCount = 0;
int revCount = 1;
float lastAngle = 0;
long lastTime = 0;
float startTime = millis();
long timeSample = 1;
float RPM = 0;
float rotPerSample = 0;
boolean runClear = false;

void setup()
{
  esc.attach(9);
  Serial.begin(1000000);
}

void loop()
{


  // Read the measured angle
  float angle = as5047p.readAngle();

  // Show the measure angle on the Serial Port
  //Serial.println(angle);


  if (angle > lastAngle + 5) {
    revCount ++;
  }

  Serial.print("revCount: "); Serial.print(revCount); Serial.print(" ");
  //Serial.print("lastAngle: "); Serial.print(lastAngle); Serial.print(" ");
  Serial.print("angle: "); Serial.print(angle); Serial.print(" ");
  Serial.println("uT");

  if (revCount % 3 == 0) {
    if (runClear) {
      esc.write(30);
      delay(3500);
      esc.write(70);
      runClear = false;
    }

  }
  else{
    runClear = true;
  }

  lastAngle = angle;

  esc.write(30);


  // interesting - throttle of 70 appears to be really close to 1000 per second



}
