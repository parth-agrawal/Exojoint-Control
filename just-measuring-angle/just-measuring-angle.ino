#include <Servo.h>
#include <AS5X47.h>
// https://github.com/Adrien-Legrand/AS5X47

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
boolean runClear = true;

void setup()
{
  Serial.begin(9600);
}

void loop()
{

 
  // Read the measured angle
  float angle = as5047p.readAngle();

  // Show the measure angle on the Serial Port
 Serial.println(angle);


  delay(50);




}
