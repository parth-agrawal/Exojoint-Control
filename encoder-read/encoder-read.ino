// void setup() {
  // put your setup code here, to run once:

// MISO = 2
// SCK = 3
// SDA = 4
// }

#include "SPI.h"
  unsigned int result = 0;
  unsigned int result1 = 0;
  unsigned int result2 = 0;

void setup ()
{
Serial.begin(9600);
SPI.begin();
SPI.setBitOrder(MSBFIRST);
}

void loop () {

digitalWrite(SS, LOW);

  //Reading 8 bit frame (the first half of 16-bit SPI transfer)
  result1 = SPI.transfer(0b00000000);
  Serial.print("byte 1: ");
  Serial.println(result1, BIN);
 
  // removing (masking) first 2 bit
  result1 &= 0b00111111;
  Serial.print("byte 1 masked: ");
  Serial.println(result1, BIN);
 
  //shifting 8 bit to left. to create emty space for last 8 bit transfer
  result1 = result1 << 8;
  Serial.print("byte 1 shifted: ");
  Serial.println(result, BIN);
 
  // getting last 8 bits (the last half of 16-bit SPI transfer)
  result2 = SPI.transfer(0b00000000);
  Serial.print("byte 2: ");
  Serial.println(result2, BIN);
 
  // merging
  result = result1 | result2;
  Serial.print("Result: ");
  Serial.print(result, BIN);
  Serial.print(" = ");
  Serial.println(result, DEC);
  Serial.println();
   
digitalWrite(SS, HIGH);
delay(1000);

}
