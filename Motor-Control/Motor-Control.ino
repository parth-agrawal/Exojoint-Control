#include <Servo.h>

Servo esc;
int throttlePin = 0;

void setup()
{
  esc.attach(9);
}

void loop()
{
  int throttlePercent = 0;
  int rawThrottle;
  if (throttlePercent > 0) {
    rawThrottle = map(throttlePercent, 1, 100, 58, 171);
  }
  else {
    rawThrottle = 20;

  }
  esc.write(rawThrottle);

  // 58 to 171

  //19.8 V 160

  //
}
