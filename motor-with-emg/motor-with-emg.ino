#include <Servo.h>
#include <AS5X47.h>
// https://github.com/Adrien-Legrand/AS5X47





#if defined(ARDUINO) && ARDUINO >= 100
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include "EMGFilters.h"

#define _DEBUG      0

#define SerialToUSB Serial

// Analog input pins
int SensorInputPins[] = {A0, A1};

#define ARR_SIZE(a) (sizeof(a) / sizeof(a[0]))

// Cycle buffer
typedef struct
{
  uint8_t index;
  uint16_t buf[64]; /* Buffer for rectified AC value */
  uint32_t sum;     /* Sum for fast caculation of mean value */
} CycleBuf_t;

// Append to cycle buffer
#define CYCLE_BUF_ADD(cb, val)                    \
  {                                               \
    cb.sum -= cb.buf[cb.index];                   \
    cb.sum += (val);                              \
    cb.buf[cb.index] = (val);                     \
    cb.index = (cb.index + 1) % ARR_SIZE(cb.buf); \
  }

/* Get mean value of cycle buffer */
#define CYCLE_BUF_MEAN(cb) (cb.sum / ARR_SIZE(cb.buf))

CycleBuf_t rectifiedAcBuf[ARR_SIZE(SensorInputPins)];

// IMPORTANT NOTE: Please unplug any power adapters when using sEMG sensors.
// Only battery powered systems an be connected directly/indirectly to sEMG
// sensors.
//
// Connect sensor to arduino board then compile & upload this sketch.
// When running, open SerilaPlot window (Menu->Tools->Serial Plotter) then
// you can see the curves.


EMGFilters myFilter[ARR_SIZE(SensorInputPins)];

// Set the input frequency.
//
// The filters work only with fixed sample frequency of
// `SAMPLE_FREQ_500HZ` or `SAMPLE_FREQ_1000HZ`.
// Inputs at other sample rates will bypass
SAMPLE_FREQUENCY sampleRate = SAMPLE_FREQ_1000HZ;

// Time interval for processing the input signal.
unsigned long long interval = 1000000ul / sampleRate;

// Set the frequency of power line hum to filter out.
//
// For countries with 60Hz power line, change to "NOTCH_FREQ_60HZ"
NOTCH_FREQUENCY humFreq = NOTCH_FREQ_50HZ;
double emgMean = 0;
double emgMovav = 0;
int i = 0;

Servo esc;
// Define where the CSN Pin in connected.
int slaveSelectPin = 10;

int throttle = 0;

// Start connection to the sensor.
AS5X47 as5047p(slaveSelectPin);
float lastRevCount = 0;
int matexRevCount = 0;
int jointRevCount = 0;
float lastJointAngle = 0;
float  jointAngle = 0;
float realAngle = 0;
float lastAngle = 0;
long lastTime = 0;
float startTime = millis();
long timeSample = 1;
float RPM = 0;
float rotPerSample = 0;
boolean runForward = false;
boolean runBackward = false;
boolean flip = false;

void setup()
{
  esc.attach(9);
  //initialization
  for (int i = 0; i < ARR_SIZE(SensorInputPins); i++) {
    myFilter[i].init(sampleRate, humFreq, true, true, true);

    rectifiedAcBuf[i].sum = 0;
    rectifiedAcBuf[i].index = 0;

    for (int j = 0; j < ARR_SIZE(rectifiedAcBuf[i].buf); j++)
    {
      rectifiedAcBuf[i].buf[j] = 0;
    }
  }

  // open serial
  SerialToUSB.begin(115200);
}

void loop()
{



  int currentTime = (millis() - startTime);

  long currentTimeSec = (millis() - startTime) / 1000;
  int RPMSampLength = 1;
  if (currentTimeSec % 3 == 0) {
    timeSample ++;
    rotPerSample = matexRevCount / currentTimeSec;
    RPM = rotPerSample * 60;
  }


  lastTime = currentTime;


  // Read the measured angle
  float angle = as5047p.readAngle();
  //delay(50);

  // Show the measure angle on the Serial Port
  //Serial.println(angle);


  if (lastAngle > angle + 2) {
    matexRevCount ++;

  }
  /*
    if (lastAngle > angle + 0.5) {
    flip = true;
    }
  */

  /*
    if(angle > lastAngle + 2){
      runForward = true;
    }
  */
  /*
    if(lastAngle > angle + 2){
    runBackward = true;
    }
    else{
    runBackward = false;
    }
  */
  /*
    if(flip && runBackward){
    matexRevCount --;
    flip = false;
    }
    if (flip) { // && !runBackward){
    matexRevCount ++;
    flip = false;
    }
  */

  jointAngle = (angle / 7) + (360 / 7) * (matexRevCount % 7);
  if (lastJointAngle > jointAngle + 2) {
    jointRevCount ++;
  }

  /*
    realAngle = angle / 7;
    matexRevCount = angle / (51);

    if (matexRevCount < 7) {
     //realAngle +=  * matexRevCount;
    }
    else {
     matexRevCount = 0;

    }

  */



  Serial.print("matexRevCount: "); Serial.print(matexRevCount); Serial.print(" ");


  Serial.print("matexRevCount: "); Serial.print(matexRevCount); Serial.print(" ");
  Serial.print("lastAngle: "); Serial.print(lastAngle); Serial.print(" ");
  Serial.print("angle: "); Serial.print(angle); Serial.print(" ");
  //Serial.print("runBackward: "); Serial.print(runBackward); Serial.print(" ");
  //Serial.print("flip: "); Serial.print(flip); Serial.print(" ");
  // Serial.print("runForward: "); Serial.print(runForward); Serial.print(" ");
  Serial.print("jointAngle: "); Serial.print(jointAngle); Serial.print(" ");
  Serial.print("lastJointAngle "); Serial.print(lastJointAngle); Serial.print(" ");
  Serial.print("jointRevCount "); Serial.print(jointRevCount); Serial.print(" ");

  //Serial.print("realAngle: "); Serial.print(realAngle); Serial.print(" ");

  //Serial.print("RPM: "); Serial.print(RPM); Serial.print(" ");
  // Serial.println("uT");


  /*
    // every 200ms, measure the angle

    if(currentTime % 2 == 0){
      lastAngle = angle;
      lastJointAngle = jointAngle;
    }
  */
  lastAngle = angle;
  lastJointAngle = jointAngle;




  // Note: `micros()` will overflow and reset every about 70 minutes.
  unsigned long long timeStamp = micros();

  // filter processing
  int data = 0, dataAfterFilter = 0;

  for (int i = 0; i <  ARR_SIZE(SensorInputPins); i++) {
    data = analogRead(SensorInputPins[i]);
    dataAfterFilter = myFilter[i].update(data);

    // Rectification
    CYCLE_BUF_ADD(rectifiedAcBuf[i], abs(dataAfterFilter));

    // Simple envelope calculation, use 2 * rectified value
    uint16_t envelope = CYCLE_BUF_MEAN(rectifiedAcBuf[i]) * 2;
    // throttle = map(envelope, 20, 70, 57, 65);
    throttle = map(envelope, 16, 60, 57, 70);

#if !_DEBUG
    //    SerialToUSB.print(128 + dataAfterFilter); // Draw offset = 128
    //    SerialToUSB.print(" ");
    //SerialToUSB.print(envelope);
    //SerialToUSB.print(envelope);

    SerialToUSB.print(" ");
    //SerialToUSB.print(throttle);
    SerialToUSB.print(" ");
#endif
  }

  unsigned long timeElapsed = micros() - timeStamp;


#if _DEBUG
  SerialToUSB.print("Filters cost time: ");
  SerialToUSB.println(timeElapsed);
#else
  if (interval > timeElapsed) {
    delay((interval - timeElapsed) / 1000);
  }
#endif

  SerialToUSB.println();


  // when arm is at back edge of case (not near the motor wires) and rotating toward the motor wires the angle reads 24. restart the program with the arm there (use the reset button on the arduino)

  if (jointAngle > 25 && jointAngle < 253) {
    esc.write(60);
  }
  else {
    esc.write(30);
  }








}
