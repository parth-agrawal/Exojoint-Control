#include <Servo.h>
 
Servo esc;
int throttlePin = 0;
 
void setup()
{
esc.attach(9);
}
 
void loop()
{
int throttle = 30;
// throttle = map(throttle, 0, 1023, 0, 179);
esc.write(throttle);

// 58 to 171

//19.8 V 160

// 
}
