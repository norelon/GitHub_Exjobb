#ifndef fukt4sensor_h
#define fukt4sensor_h
#include "arduino.h"

const int OUT_PIN = A3;
const int IN_PIN = A2;



int moist(bool no_write = 1)
{
  static float variance = 0.038688779;  //Uppmätt värde efter 200 samplingar utan Kalman
  static float varianceProcess = 1e-8; //snabbhet i systemet
  static float Pc = 0.0;
  static float G = 0.0;
  static float P = 1.0;
  static float Xp = 0.0;
  static float Zp = 0.0;
  static float Xe = 55.0;             //startvärde
  static const float IN_STRAY_CAP_TO_GND = 24.48;
  static const float IN_CAP_TO_GND  = IN_STRAY_CAP_TO_GND;
  static const float R_PULLUP = 34.8;  
  static const int MAX_ADC_VALUE = 1023;
  while(1)
  {
    pinMode(IN_PIN, INPUT);
    digitalWrite(OUT_PIN, HIGH);
    int val = analogRead(IN_PIN);
    digitalWrite(OUT_PIN, LOW);

    if (val < 1000)
    {
      pinMode(IN_PIN, OUTPUT);

      float capacitance = (float)val * IN_CAP_TO_GND / (float)(MAX_ADC_VALUE - val);
      float humidity=((capacitance-330)/4)+55;

      
      //Kalmanprocess
      Pc = P + varianceProcess;
      G = Pc / (Pc + variance);
      P = (1 - G) * Pc;
      Xp = Xe;
      Zp = Xp;
      Xe = G * (humidity - Zp) + Xp;
      
      //Serial.print(F("Capacitance Value = "));
      //Serial.print(capacitance, 3);
      //Serial.print(F(" pF ("));
      if (no_write == 0){
        Serial.print("\t");
        //Serial.print(val);
        //Serial.print(F(") "));
        Serial.print(humidity);
        Serial.print("\t");
        Serial.print(Xe);
      }
      return Xe;
    }
    else
    {
      pinMode(IN_PIN, OUTPUT);
      delay(1);
      pinMode(OUT_PIN, INPUT_PULLUP);
      unsigned long u1 = micros();
      unsigned long t;
      int digVal;

      do
      {
        digVal = digitalRead(OUT_PIN);
        unsigned long u2 = micros();
        t = u2 > u1 ? u2 - u1 : u1 - u2;
      } while ((digVal < 1) && (t < 400000L));

      pinMode(OUT_PIN, INPUT);  
      val = analogRead(OUT_PIN);
      digitalWrite(IN_PIN, HIGH);
      int dischargeTime = (int)(t / 1000L) * 5;
      delay(dischargeTime);   
      pinMode(OUT_PIN, OUTPUT);  
      digitalWrite(OUT_PIN, LOW);
      digitalWrite(IN_PIN, LOW);

      float capacitance = -(float)t / R_PULLUP
                              / log(1.0 - (float)val / (float)MAX_ADC_VALUE);
      if(no_write == 0){
        Serial.print(F("fuktsensor:\t kapacitans ="));
        if (capacitance > 1000.0)
        {
          Serial.print(capacitance / 1000.0, 2);
          Serial.print(F(" uF"));
        }
        else
        {
          Serial.print(capacitance, 2);
          Serial.print(F(" nF"));
        }
  
        Serial.print(F(" ("));
        Serial.print(digVal == 1 ? F("Normal") : F("HighVal"));
        Serial.print(F(", t= "));
        Serial.print(t);
        Serial.print(F(" us, ADC= "));
        Serial.print(val);
        Serial.println(F(")"));
      }
      return val;
    }
    while (millis() % 1000 != 0)
      ;
  }   
}
#endif
