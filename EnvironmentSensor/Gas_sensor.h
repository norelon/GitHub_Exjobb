#ifndef gassensor_h
#define gassensor_h
#include "arduino.h"



// The loop routine runs over and over again forever
float gas(bool no_write = 1) {
  //Kalmanvariabler
  static float variance = 71.6259296;  //Uppmätt värde efter 200 samplingar utan Kalman
  static float varianceProcess = 1e-5; //snabbhet i systemet
  static float Pc = 0.0;
  static float G = 0.0;
  static float P = 1.0;
  static float Xp = 0.0;
  static float Zp = 0.0;
  static float Xe = 400.0;             //startvärde
  // Analog pin 0 will be called 'sensor'
  static int sensor = A5;
  // Set the initial sensorValue to 0
  static float sensorValue = 0;
  static float ppm = 0;
  
  sensorValue = analogRead(sensor);
  sensorValue = (sensorValue / 1000) + 0.126; //Kalibrering efter 350 ppm utomhus
  ppm = ((sensorValue - 0.33) / 0.000066);    //Uppskattning av formel från datablad

    //Kalmanprocess
  Pc = P + varianceProcess;
  G = Pc / (Pc + variance);
  P = (1 - G) * Pc;
  Xp = Xe;
  Zp = Xp;
  Xe = G * (ppm - Zp) + Xp;

  if (no_write == 0){
    Serial.print("\t");
    Serial.print(ppm);
    //Serial.println("");
    Serial.print("\t");
    Serial.print(Xe);
  }
  return Xe;
}
#endif
