#ifndef pirsensor_h
#define pirsensor_h
#include "arduino.h"

byte PIR_PIN = 2;
byte LED_PIN = 4;
int pirhigh = 0;

// We need to declare the data exchange
// variable to be volatile - the value is
// read from memory.
volatile int isrpir = 0;


void pir() {
  long int PirTid = 0;
  Serial.println("Pirsensor:\tDetektion");
  pirhigh++;
  /*while (PIR_PIN){                            //Så länge insignalen är hög
    if (PirTid == 0)                          //För att skriva "Detektion" en gång
    {
        
    }
    PirTid++;                                 //Ökar PirTid variablen med ett, för tidsmätning
    delay(1);                                 //Fördröjning i 1 ms
  }
  if (PirTid >= 200) {
    Serial.print("Varade i ");                //Skriver text i monitorn
    Serial.print(PirTid);                     //Skriver värdet på tiden
    Serial.println(" ms.");                   //Skriver text i monitorn
  }*/

}


void interrupt_init()
{
  Serial.println("Initializing interrupts");
  attachInterrupt(digitalPinToInterrupt(PIR_PIN), pir, RISING);
}

#endif
