#ifndef voltage_check_h
#define voltage_check_h
#include "arduino.h"


// We need to declare the data exchange
// variable to be volatile - the value is
// read from memory.
void volt(bool no_write = 1) {
  float voltage5 = analogRead(1);
  
  float voltage33 = analogRead(0);
  Serial.print("Voltage5:\t");
  Serial.print(voltage5);
  Serial.print("\t");
  voltage5 = voltage5 * (4.59 / 1003.0);
  Serial.println(voltage5);
  Serial.print("Voltage33:\t");
  Serial.print(voltage33);
  Serial.print("\t");
  voltage33 = voltage33 * (4.59 / 1003.0);
  Serial.println(voltage33);
}


#endif
