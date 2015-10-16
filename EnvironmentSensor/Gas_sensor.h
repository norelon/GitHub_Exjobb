#ifndef gassensor_h
#define gassensor_h
#include "arduino.h"

int GAS_PIN = A5;

float gas(bool no_write = 1) {

  //Kalmanvariabler
  static float variance = 71.6259296;           //Uppmätt värde efter 200 samplingar utan Kalman
  static float varianceProcess = 1e-5;          //Snabbhet i systemet
  static float Pc = 0.0;
  static float G = 0.0;
  static float P = 1.0;
  static float Xp = 0.0;
  static float Zp = 0.0;
  
  static float sensorValue = 0;
  static float ppm = 0;

  sensorValue = analogRead(GAS_PIN);            //Läs analoga värdet
  sensorValue = (sensorValue / 1000) + 0.175;   //Beräkning till V samt kalibrering enligt 350 ppm utomhus
  ppm = ((sensorValue - 0.33) / 0.000066);      //Uppskattning av formel från V till PPM enligt datablad
  static float Xe = ppm;                      //Startvärde kalman
  //Kalmanprocess
  Pc = P + varianceProcess;
  G = Pc / (Pc + variance);
  P = (1 - G) * Pc;
  Xp = Xe;
  Zp = Xp;
  Xe = G * (ppm - Zp) + Xp;

  if (no_write == 0) {
    Serial.print("\t");
    Serial.print(ppm);
    //Serial.println("");
    Serial.print("\t");
    Serial.print(Xe);
  }
  return Xe;
}
#endif
