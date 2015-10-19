#ifndef odds_h
#define odds_h
#include "arduino.h"

float odds(int pir, float temperatur, float co2 = 0, int ljud = 0, float ljus = 0)
{
    static const int antal_samples = 2;      //samples mellan derivata mätning ~35s mellan varje
    
    //viktning samt statisktiskt undersökt data för de olika sensorerna.
    const float pir_high = 1;
    const float pir_low = -0.05;
    const float pir_weight = 1;
    const float pir_max_odds = 1; //uppmätt
    float pir_odds = 0;

    const float temp_weight = 6;
    const float temp_max_odds = 0.7; //uppmätt
    float temp_odds = 0;
    bool temp_left = 0;

    static float total_odds = 0;

    static float temp_samples[antal_samples];
    static unsigned int counter = 0;
    
    if(counter < antal_samples){
      counter++;
    }
    
    temp_samples[antal_samples] = temperatur;     //sparar gamla temperatur värden.
    for(int i = 0; i < antal_samples; i++) 
    {
       temp_samples[i] = temp_samples[i+1];
       
    }
    
    temp_odds = temp_weight*(temp_samples[antal_samples-1]-temp_samples[0]);
    
    if(pir >= 1) pir_odds = pir_high*pir_weight;
    else pir_odds = pir_low*pir_weight;
    
    total_odds += (pir_odds + temp_odds);
    if (total_odds >= 1) 
    {
      total_odds = 1;
      Serial.println("total_odds för stor");
    }
    else if (total_odds < 0) 
    {
      total_odds = 0;
      Serial.println("total_odds negativ");
    }

    
    Serial.print("temp:");
    Serial.print(temp_odds);
    Serial.print("\tpir:");
    Serial.println(pir_odds);
    Serial.print("odds:\t");
    Serial.println(total_odds);
    return total_odds;
    
}

#endif
