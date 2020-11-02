#include <Servo.h>
#include <AS5X47.h>
// https://github.com/Adrien-Legrand/AS5X47

Servo esc;
int emgPin = A3;

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
  esc.attach(9);
  Serial.begin(9600);
}

void loop()
{
/*
  int emgValue = analogRead(emgPin);

  // rectifier
  int zeroPt = 321;
  emgValue = emgValue - zeroPt;
  if(emgValue < 0){
    emgValue *= -1;
  }
*/


  
  int throttle = map(emgValue, 400, 1023, 57, 63);
  Serial.print("throttle: "); Serial.print(throttle); Serial.print(" ");
  Serial.print("emgValue: "); Serial.print(emgValue); Serial.print(" ");
  Serial.println("");


  /*
    float currentTime = (millis() - startTime);

    long currentTimeSec = (millis() - startTime) / 1000;
    int RPMSampLength = 1;
    if (currentTimeSec % 3 == 0) {
    timeSample ++;
    rotPerSample = revCount / currentTimeSec;
    RPM = rotPerSample * 60;
    }

    lastTime = currentTime;


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
    Serial.print("RPM: "); Serial.print(RPM); Serial.print(" ");
    Serial.println("uT");


    lastAngle = angle;
  */
  esc.write(throttle);





}
