#ifndef temp2sensor_h
#define temp2sensor_h
#include "arduino.h"

byte PWM_PIN = 13;                                  //Deklarerar ingång för tempsensor

float temperature(int numberofvaliddutycycles = 385, bool no_write = 1) { //Framtestat stabilitetsvärde
  float pwm_high;
  float pwm_low;
  float dutycycle = 0;
  float sumdutycycle = 0;
  float temperature = 0;
  float validdutycycle = 0;
  float totalsum = 0;
  unsigned int count = 0;

  //Kalmanvariabler
  static float variance = 0.045168894;            //Uppmätt värde efter 100 samplingar utan Kalman, gammalt 0.010536689
  static float varianceProcess = 1e-5;            //Snabbhet i systemet
  static float Pc = 0.0;
  static float G = 0.0;
  static float P = 1.0;
  static float Xp = 0.0;
  static float Zp = 0.0;
  static float Xe = 0.0;

  while (1)
  {
    //digitalWrite(OUT_PIN, HIGH);
    pwm_low = pulseIn(PWM_PIN, LOW);              //Returnerar tiden låg i ms, börjar på låg
    pwm_high = pulseIn(PWM_PIN, HIGH);            //Returnerar tiden hög i ms
    dutycycle = ((pwm_high) / (pwm_low + pwm_high)); //Antalet procent av duty cycle som hög
    validdutycycle = validdutycycle + dutycycle;  //Summerar procentsatserna
    sumdutycycle++;

    if (sumdutycycle == 8) {                      //8 duty cycles har gått
      validdutycycle = validdutycycle / 8;        //Medelvärdesberäkna
      totalsum = totalsum + validdutycycle;       //Summerar medelvärdena
      count++;                                    //För antalet "valid duty cycles" vi ska gå
      sumdutycycle = 0;
      validdutycycle = 0;
    }

    if (count == numberofvaliddutycycles) {                                 //Valda antalet valid duty cycles är inlästa
      totalsum = totalsum / numberofvaliddutycycles;                        //Medelvärdesberäkna procenten hög av alla duty cycles
      temperature = (((-2.42 * (totalsum * totalsum)) + (215.63 * totalsum) - 68.83))-3; //Temperaturberäkning från datablad

      //Kalmanprocess
      Pc = P + varianceProcess;
      G = Pc / (Pc + variance);
      P = (1 - G) * Pc;
      Xp = Xe;
      Zp = Xp;
      Xe = G * (temperature - Zp) + Xp;

      if (no_write == 0) {
        Serial.print(temperature);        //Skriver temperaturen i monitorn
        Serial.print("\t");
        Serial.print(Xe);                 //Skriver Kalmantemperaturen i monitorn
      }
      totalsum = 0;
      count = 0;
      return Xe;
    }
  }
}

#endif
