#include <Servo.h>
#include <AS5X47.h>
// https://github.com/Adrien-Legrand/AS5X47

Servo esc;
int emgPin = A0;

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
int throttle = 0;
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

  int emgValue = analogRead(emgPin);

  /*
    // rectifier
    int zeroPt = 300;
    emgValue = emgValue - zeroPt;
    if (emgValue < 0) {
      emgValue *= -1;
    }

    int meanSamples = 10;


  */






  /*
    emgMean = emgMean + emgValue;
    if(i == meanSamples){
      emgMean = emgMean / meanSamples;
      i = 0;
    }
    else{
      i ++;
    }
  */


  /*

    emgMovav = emgMovav * 0.80 + abs(emgValue) * 0.20; // Rectified and smoothed signal

    int throttle = map(emgMovav, 30, 80, 57, 60);
    Serial.print("throttle: "); Serial.print(throttle); Serial.print(" ");
    Serial.print("EMG Reading: "); Serial.print(emgMovav); Serial.print(" ");
    Serial.println("");

  */


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
    throttle = map(envelope, 20, 60, 57, 70);
    
#if !_DEBUG
    //    SerialToUSB.print(128 + dataAfterFilter); // Draw offset = 128
    //    SerialToUSB.print(" ");
    // SerialToUSB.print(envelope);
    SerialToUSB.print(envelope);

    SerialToUSB.print(" ");
    SerialToUSB.print(throttle);
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

  esc.write(throttle);





}
