#ifndef inut_h
#define inut_h
#include "arduino.h"

int inut_set = 0;
int inut_out = 0;
int inut_in = 0;
int inut_tot = 0;
volatile int isrinut = 0;

void inut() {
  if ((digitalRead(5) == LOW) && (digitalRead(4) == LOW)) {
    delay(50);
    inut_set=0;
  }
  if ((digitalRead(5) == HIGH) && (inut_set == 0)) {
    inut_out++;
    inut_tot=inut_in-inut_out;
    inut_set = 1;
  }
  if ((digitalRead(4) == HIGH) && (inut_set == 0)) {
    inut_in++;
    inut_tot=inut_in-inut_out;
    inut_set = 1;
  }
}
#endif
