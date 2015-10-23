#ifndef odds_h
#define odds_h
#include "arduino.h"

float odds(int pir, float temperatur, float co2, float ljud, float ljus)
{
    const int use_temp = 2;                  //antalsample innan temperaturen/co2 räknas 
    
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
    static float co2_ancient = 0;

    const float ljus_odds = 0.001;     //odds att den går upp om nån är där
    float ljus_weight = 0;                   //ner med närvaro händer aldrig.
    static float ljus_old = 0;
    static int ljus_counter = 0;
    ljus *= 0.01;                  //för att få det i räknbar procent.

    static unsigned int one_samples = 0;
    static unsigned int total_samples = 0;
    static unsigned int pir_samples = 0;
    static unsigned int co2_samples = 0;
    static unsigned int temp_samples_up = 0;
    static unsigned int temp_samples_down = 0;
    static unsigned int temp_samples_still = 0;
    static unsigned int ljud_samples = 0;
    total_samples++;
    if(inut_tot >= 1){
      one_samples++;
    }
    
    
    static float total_odds = 0;
    
    
    static unsigned int counter = 0;

    ljus_weight = 0;                       //påvisar inget samband då det är ljust
    if (ljus < 0.50){                      //56.3 lägsta innan allt släkts på företaget.
       const float ljus_odds = 0.001;//odds att den går upp om nån är där
       ljus_weight = (1-ljus);           //ner med närvaro händer aldrig.
    }
    

         
    if(counter < use_temp){
      counter++;
      temp_weight = 0;    //litar vi på den?
      co2_weight = 0;    //litar vi på den?
      temp_old = temperatur;
      co2_old = co2;
      co2_ancient = co2_old;
      ljus_old = ljus;
    }
    else{
      //beräknar data för temperatur sensorn
      float ljusval = (ljus-ljus_old);
      ljus_old = ljus;
      temp_weight = 1;
      if(ljusval > 0.1 && ljus_old > 0.6){  //anti lys
        ljus_counter = 10;
      }
      if(ljus_counter > 0){
        ljus_counter--;
        temp_weight = 0; 
      }
      float tempval = (temperatur-temp_old);
      temp_old = temperatur;
      if (tempval < 0){
        temp_odds = temp_odds_down;
        temp_samples_down++;
      }
      else if(tempval >= 0.01){  
        temp_odds = temp_odds_up;
        temp_samples_up++;
      }
      else{
       temp_odds = temp_odds_still;
       temp_samples_still++;
      }

      //beräknar odds för co2
      float co2val = (co2-co2_old);
      co2_weight = 1;
      if(co2val == 0){    //om den motförmodan blir 0 
        co2val = (co2-co2_ancient);
      }
      co2_old = co2;
      co2_ancient = co2_old;

      if (co2val < 0){
        co2_odds = co2_odds_down;
      }
      else{
        co2_odds = co2_odds_up;
        co2_samples++;
      }
    }

    //ljud sensor odds
    if (ljud > 730){//(730/630)*microphone_Xe)
      ljud_samples++;
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
      pir_samples++;
    }
    else {
      pir_weight = 1;
      pir_odds = pir_odds_not;
    }
    
    total_odds = ((pir_odds*pir_weight + temp_odds*temp_weight+ co2_odds*co2_weight+ljud_odds*ljud_weight+ljus_odds*ljus_weight)/(ljus_weight+ljud_weight+co2_weight+pir_weight+temp_weight));
    if (total_odds > 1){
      
      Serial.print("total_odds för stor:");
      Serial.println(total_odds);
      total_odds = 1;
    }
    else if (total_odds < 0){
      total_odds = 0;
      Serial.println("total_odds negativ");
    }

    Serial.print("odds: ");
    Serial.print(total_odds);
    Serial.print("\t\tco2:");
    Serial.print(co2_odds);
    Serial.print("\tljud:");
    Serial.print(ljud_odds);
    Serial.print("\ttemp:");
    Serial.print(temp_odds);
    Serial.print("\tpir:");
    Serial.println(pir_odds);
    
    return total_odds;
    
}

#endif
