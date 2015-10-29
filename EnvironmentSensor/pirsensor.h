#ifndef pirsensor_h
#define pirsensor_h
#include "arduino.h"

byte PIR_PIN = 2;
volatile int pirhigh = 0;

void pir() {      //Pir-sensorn får ett interrupt.
  pirhigh++;
}

#endif
