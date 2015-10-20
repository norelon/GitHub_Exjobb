#ifndef odds_h
#define odds_h
#include "arduino.h"

float odds(int pir, float temperatur, float co2 = 0, int ljud = 0, float ljus = 0)
{
    const int use_temp = 1;                  //antalsample innan temperaturen räknas 
    
    //viktning samt statisktiskt undersökt data för de olika sensorerna.
    const float odds_there = 0.108;      //totalt att någon är där
    
    const float pir_max_odds = 1;       //gissar rätt om nån är där
    const float pir_miss_odds = 0.3571; //(25/70)låter bli att gissa att nån är där.    
    const float pir_odds_high = pir_max_odds;
    const float pir_odds_low = 0.001;   //oddsen att det är någon där om den är låg
    float pir_weight = 1;
    float pir_odds = 0;

    const float temp_odds_up = 0.8571;    //(60/70)odds att den går upp om nån är där ()
    const float temp_odds_upno = 0.0346;  //(20/578)odds att den går upp om ingen är där
    const float temp_odds_down = 0;       //ner med närvaro händer aldrig.
    const float temp_odds_downno = 0.3287;     //odds att den går ner om ingen är där
    const float temp_odds_still = 0.1415; //odds att den står stilla om någon är där 0.1415
    const float temp_odds_stillno = 0.6367;
    float temp_weight = 0;    //litar vi på den?
    const float temp_adjust = 20;
    float temp_odds = 0;
    static float temp_old = 0;
    bool temp_left = 0;
    static float total_odds = 0;
    
    
    static unsigned int counter = 0;
    
    if(counter < use_temp){
      counter++;
      temp_weight = 0;    //litar vi på den?
    }
    else{
      float tempval = temp_adjust*(temperatur-temp_old);
      temp_old = temperatur;
      if (tempval < 0){
        temp_odds = temp_odds_down;
        temp_weight = (1-temp_odds_downno);
      }
      else if ((temp_weight+tempval) >= (1-temp_odds_upno)){
        temp_odds = temp_odds_up;
        temp_weight = (1-temp_odds_upno);
      }
      else if(tempval >= temp_odds_still){
        temp_weight += tempval;
        temp_odds = temp_odds_up;
      }
      /*else if ((temp_weight+tempval) >= (1-temp_odds_stillno)){
       temp_odds = temp_odds_still;
       temp_weight += tempval;
      }*/
      else{
       temp_odds = temp_odds_still;
       temp_weight = (1-(temp_odds_stillno));
      }
    }
    

    if(pir >= 1){
      pir_weight = pir_max_odds;
      pir_odds = pir_odds_high;
    }
    else {
      pir_weight = pir_miss_odds;
      pir_odds = pir_odds_low;
    }
    
    total_odds = ((pir_odds*pir_weight + temp_odds*temp_weight)/(pir_weight+temp_weight));
    if (total_odds > 1){
      
      Serial.print("total_odds för stor:");
      Serial.println(total_odds);
      total_odds = 1;
    }
    else if (total_odds < 0){
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
