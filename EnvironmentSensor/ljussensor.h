#ifndef ljussensor_h
#define ljussensor_h
#include "arduino.h"

int lightPin = 1;

float light() {
  static float lightLevel = 0;
  static float lightMax = 1023;                                         //Testat maxvärde 1003, teoretiskt max = 1023
  static float lightMin = 0;                                            //Testat 20 vid totalt mörker, teoretiskt minimum = 0
  lightLevel = analogRead(lightPin);
  lightLevel = ((lightLevel - lightMin) / (lightMax - lightMin)) * 100; //skickar ut data som "läsbar" procent, dvs *100.  
  return (lightLevel);
}

#endif
