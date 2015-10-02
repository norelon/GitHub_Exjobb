#ifndef ljussensor_h
#define ljussensor_h
#include "arduino.h"

int lightPin = 1;

float light() {
  static float lightLevel = 0;
  static float lightMax = 1023;  //Testat 1003
  static float lightMin = 0;     //Testat 20
  lightLevel = analogRead(lightPin);
  lightLevel = ((lightLevel - lightMin) / (lightMax - lightMin)) * 100;
  return (lightLevel);
}

#endif
