#ifndef pirsensor_h
#define pirsensor_h
#include "arduino.h"

byte PIR_PIN = 2;
int pirhigh = 0;

// We need to declare the data exchange
// variable to be volatile - the value is
// read from memory.
volatile int isrpir = 0;


void pir() {
  long int PirTid = 0;
  //Serial.println("Pirsensor:\tDetektion");
  pirhigh++;
}

#endif
