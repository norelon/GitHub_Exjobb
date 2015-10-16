#ifndef inut_h
#define inut_h
#include "arduino.h"

volatile int inut_tot = 0;
volatile int isrinut = 0;
static int inut_set = 0;
static int inut_out = 0;
static int inut_in = 0;

void inut() {

  noInterrupts();

  if ((digitalRead(5) == HIGH) && (inut_set == 0)) {
    inut_set = 1;
    //inut_out++;
    //inut_tot = inut_in - inut_out;
    inut_tot--;
  }
  if ((digitalRead(4) == HIGH) && (inut_set == 0)) {
    inut_set = 1;
    inut_tot++;
    //inut_in++;
    //inut_tot = inut_in - inut_out;
  }

  inut_set = 0;
  interrupts();
}
#endif
