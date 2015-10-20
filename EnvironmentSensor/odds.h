#ifndef odds_h
#define odds_h
#include "arduino.h"

float odds(int pir, float temperatur, float co2, float ljud, float ljus = 0)
{
    const int use_temp = 1;                  //antalsample innan temperaturen räknas 
    
    //viktning samt statisktiskt undersökt data för de olika sensorerna.
    const float odds_there = 0.108;      //totalt att någon är där
    
    const float pir_odds_there = 1;      //oddsen att någon är där vid detektion
    const float pir_odds_not = 0.3913;   //oddsen att det är någon där då det inte är detektion
    float pir_weight = 1;
    float pir_odds = 0;

    const float ljud_odds_there = 0.9711;      //oddsen att någon är där vid detektion
    const float ljud_odds_not = 0.4857;   //oddsen att det är någon där då det inte är detektion
    float ljud_weight = 1;
    float ljud_odds = 0;

    const float temp_odds_up = 0.9612;     //odds att den går upp om nån är där
    const float temp_odds_still = 0.1833;  //odds att den står stilla om någon är där 0.1415
    const float temp_odds_down = 0;        //ner med närvaro händer aldrig.
    float temp_weight = 0;    //litar vi på den?
    float temp_odds = 0;
    static float temp_old = 0;
   
    const float co2_odds_up = 0.5761;     //odds att den går upp om nån är där
    const float co2_odds_down = 0.1840;        //ner med närvaro händer aldrig.
    float co2_weight = 0;    //litar vi på den?
    float co2_odds = 0;
    static float co2_old = 0;
    
    static float total_odds = 0;
    
    
    static unsigned int counter = 0;
    
    if(counter < use_temp){
      counter++;
      temp_weight = 0;    //litar vi på den?
      co2_weight = 0;    //litar vi på den?
    }
    else{
      //beräknar data för temperatur sensorn
      float tempval = (temperatur-temp_old);
      temp_old = temperatur;
      if (tempval < 0){
        temp_odds = temp_odds_down;
        temp_weight = 1;
      }
      else if(tempval >= 0.01){
        temp_weight = 1;
        temp_odds = temp_odds_up;
      }

      else{
       temp_odds = temp_odds_still;
       temp_weight = 1;
      }

      //beräknar odds för co2
      float co2val = (temperatur-temp_old);
      co2_weight = 1;
      co2_old = co2;
      if (tempval < 0){
        co2_odds = co2_odds_down;
      }
      else{
       co2_odds = co2_odds_up;
      }
    }

    //ljud sensor odds
    if (ljud > 730){//(730/630)*microphone_Xe)
      ljud_odds = ljud_odds_there;
      ljud_weight = 1;
    }
    else{
      ljud_odds = ljud_odds_not;
      ljud_weight = 1;
    }

    if(pir >= 1){
      pir_weight = 1;
      pir_odds = pir_odds_there;
    }
    else {
      pir_weight = 1;
      pir_odds = pir_odds_not;
    }
    
    total_odds = ((pir_odds*pir_weight + temp_odds*temp_weight+ co2_odds*co2_weight+ljud_odds*ljud_weight)/(ljud_weight+co2_weight+pir_weight+temp_weight));
    if (total_odds > 1){
      
      Serial.print("total_odds för stor:");
      Serial.println(total_odds);
      total_odds = 1;
    }
    else if (total_odds < 0){
      total_odds = 0;
      Serial.println("total_odds negativ");
    }

    Serial.print("co2:");
    Serial.print(co2_odds);
    Serial.print("\tljud:");
    Serial.print(ljud_odds);
    Serial.print("\ttemp:");
    Serial.print(temp_odds);
    Serial.print("\tpir:");
    Serial.println(pir_odds);
    Serial.print("odds:\t");
    Serial.println(total_odds);
    return total_odds;
    
}

#endif
