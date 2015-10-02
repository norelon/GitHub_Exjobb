#ifndef pirsensor_h
#define pirsensor_h
#include "arduino.h"

byte PIR_PIN = 2;
volatile int pirhigh = 0;

volatile int isrpir = 0;

void pir() {
  //Serial.println("Pirsensor:\tDetektion");
  pirhigh++;
}

#endif
