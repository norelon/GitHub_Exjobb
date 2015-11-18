#ifndef odds_h
#define odds_h
#include "arduino.h"
#include "../EEPROM/EEPROM.h"

float odds(int pir, float temperatur, float co2, float ljud, float ljus)
{
  const int use_temp = 2;                 //antalsample innan temperaturen/co2 räknas (derivata problem)
  const unsigned EE_safecode = 13;      //mindre värde än 65536, används för att se så att det är rätt version/data på EEPROM


  //viktning samt statisktiskt undersökt data för de olika sensorerna.
  static bool log_data = 0;               //har knappen blivit tryckt, använd samples för att beräkna odds.

  static int nu_inut_tot = 0;
  static int old_inut_tot = 0;

  old_inut_tot = nu_inut_tot;
  nu_inut_tot = inut_tot;

  static unsigned counter = 0;            //räknar antalet körningar.
  static unsigned int counter_in = 0;     //antal samples med någon inne.
  if (nu_inut_tot >= 1) counter_in++;
  counter++;

  static float pir_odds_there = 1;         //oddsen att någon är där vid detektion
  static float pir_odds_not = 0.3913;      //oddsen att det är någon där då det inte är detektion
  float pir_weight = 1;                    //hur mycket vi litar på sensorn, ändras dynamiskt
  float pir_odds = 0;
  static unsigned counter_pir_there = 0;   //antalet samples som pir har varit hög vid närvaro
  static unsigned counter_pir_not = 0;     //antalet samples som pir har varit hög vid frånvaro

  static float ljud_odds_there = 0.9711;   //oddsen att någon är där vid detektion
  static float ljud_odds_not = 0.4857;     //oddsen att det är någon där då det inte är detektion
  float ljud_weight = 1;                   //hur mycket vi litar på sensorn, ändras dynamiskt
  float ljud_odds = 0;
  static unsigned counter_ljud_there = 0;  //räknar antalet samples som ljud har varit hög vid närvaro
  static unsigned counter_ljud_not = 0;    //antalet samples som ljud varit hög vid frånvaro

  static float temp_odds_up = 0.9612;      //odds att den går upp om nån är där
  static float temp_odds_still = 0.1833;   //odds att den står stilla om någon är där 0.1415
  static float temp_odds_down = 0;         //ner med närvaro händer aldrig.
  float temp_weight = 0;                   //hur mycket vi litar på sensorn, ändras dynamiskt
  float temp_odds = 0;
  static float temp_old = 0;
  static unsigned counter_temp_up = 0;     //antalet samples som temp har stigt vid närvaro
  static unsigned counter_temp_upnot = 0;  //antalet samples som temp har stigt vid frånvaro
  static unsigned counter_temp_still = 0;  //antalet samples som temp har stått stilla vid närvaro
  static unsigned counter_temp_stillnot = 0;//antalet samples som temp har stått stilla vid frånvaro
  static unsigned counter_temp_down = 0;   //antalet samples som temp har sjunkt vid närvaro
  static unsigned counter_temp_downnot = 0;//antalet samples som temp har sjunkt vid frånvaro

  static float co2_odds_up = 0.5761;       //odds att den går upp om nån är där
  static float co2_odds_down = 0.1840;     //ner med närvaro händer aldrig.
  float co2_weight = 0;                    //hur mycket vi litar på sensorn, ändras dynamiskt
  float co2_odds = 0;
  static float co2_old = 0;
  static float co2_ancient = 0;
  static unsigned counter_co2_up = 0;     //antalet samples som co2 har stigt vid närvaro
  static unsigned counter_co2_upnot = 0;  //antalet samples som co2 har stigt vid frånvaro
  static unsigned counter_co2_down = 0;   //antalet samples som co2 har sjunkt vid närvaro
  static unsigned counter_co2_downnot = 0;//antalet samples som co2 har sjunkt vid frånvaro


  static  float ljus_odds = 0.001;        //odds att den går upp om nån är där
  float ljus_weight = 0;                  //ner med närvaro händer aldrig.
  static float ljus_old = 0;
  static int ljus_counter = 0;
  ljus *= 0.01;                           //för att få det i räknbar procent.
  static unsigned counter_ljus_there = 0; //antalet samples som ljus har haft närvaro vid mörker
  static unsigned counter_ljus_not = 0;

  static float total_odds = 0.5;
  static float total_odds_old = 0.5;
  static float total_odds_moment = 0.5;

  static bool calculate_odds = 0;
  
  if ((counter_in >= 1 && counter_in != counter) && (calculate_odds == 0 || nu_inut_tot != old_inut_tot)) //fel kontroll så att oddsen aldrig kan delas med 0
  {
    //beräknar oddsen för närvaro enligt valda sensorer
    
    if (counter_pir_there >= 1) {               //felkontroll så att oddsen aldrig kan delas med 0
      pir_odds_there = (float(counter_pir_there) / float(counter_in)) /
                       ((float(counter_pir_there) / float(counter_in)) + (float(counter_pir_not) / (float(counter) - float(counter_in))));
      pir_odds_not = (1 - (float(counter_pir_there) / float(counter_in))) /
                     ((1 - (float(counter_pir_there) / float(counter_in))) + (1 - (float(counter_pir_not) / (float(counter) - float(counter_in)))));
      calculate_odds = 1;
    }

    if (counter_ljud_there >= 1) {              //felkontroll så att oddsen aldrig kan delas med 0
      ljud_odds_there = (float(counter_ljud_there) / float(counter_in)) /
                        ((float(counter_ljud_there) / float(counter_in)) + (float(counter_ljud_not) / float(counter - counter_in)));
      ljud_odds_not = (1 - (float(counter_ljud_there) / float(counter_in))) /
                      ((1 - (float(counter_ljud_there) / float(counter_in))) + (1 - (float(counter_ljud_not) / (float(counter) - float(counter_in)))));
    }

    if (counter_co2_up >= 1 && counter_co2_downnot >= 1) { //felkontroll så att oddsen aldrig kan delas med 0
      co2_odds_up = (float(counter_co2_up) / float(counter_in)) /
                    ((float(counter_co2_up) / float(counter_in)) + (float(counter_co2_upnot) / (float(counter) - float(counter_in))));
      co2_odds_down = (float(counter_co2_down) / float(counter_in)) /
                      ((float(counter_co2_down) / float(counter_in)) + (float(counter_co2_downnot) / (float(counter) - float(counter_in))));
    }

    if (counter_temp_up >= 1 && counter_temp_downnot >= 1 && counter_temp_stillnot >= 1) { //fel kontroll så att oddsen aldrig kan delas med 0
      temp_odds_up = (float(counter_temp_up) / float(counter_in)) /
                     ((float(counter_temp_up) / float(counter_in)) + (float(counter_temp_upnot) / (float(counter) - float(counter_in))));
      temp_odds_down = (float(counter_temp_down) / float(counter_in)) /
                       ((float(counter_temp_down) / float(counter_in)) + (float(counter_temp_downnot) / (float(counter) - float(counter_in))));
      temp_odds_still = (float(counter_temp_still) / float(counter_in)) /
                        ((float(counter_temp_still) / float(counter_in)) + (float(counter_temp_stillnot) / (float(counter) - float(counter_in))));
    }

    if (counter_ljus_not >= 1) {                //felkontroll så att oddsen aldrig kan delas med 0
      ljus_odds = (float(counter_ljus_there) / float(counter_in)) / ((float(counter_ljus_there) / float(counter_in)) + (float(counter_ljus_not) / float(counter - counter_in)));
      //ljus_odds_not = (1-(float(counter_ljus_there) / float(counter_in)))/((1-(float(counter_ljus_there) / float(counter_in))) + (1-(float(counter_ljus_not) / (float(counter)-float(counter_in)))));
    }

  }

  if (log_data == 0 && lowByte(EE_safecode) == EEPROM.read(0) && highByte(EE_safecode) == EEPROM.read(1)) {
    counter_pir_there += EEPROM.read(2) + 256 * EEPROM.read(3);
    counter_pir_not += EEPROM.read(4) + 256 * EEPROM.read(5);
    counter_ljud_there += EEPROM.read(6) + 256 * EEPROM.read(7);
    counter_ljud_not += EEPROM.read(8) + 256 * EEPROM.read(9);
    counter_temp_up += EEPROM.read(10) + 256 * EEPROM.read(11);
    counter_temp_upnot += EEPROM.read(12) + 256 * EEPROM.read(13);
    counter_temp_still += EEPROM.read(14) + 256 * EEPROM.read(15);
    counter_temp_stillnot += EEPROM.read(16) + 256 * EEPROM.read(17);
    counter_temp_down += EEPROM.read(18) + 256 * EEPROM.read(19);
    counter_temp_downnot += EEPROM.read(20) + 256 * EEPROM.read(21);
    counter_co2_up += EEPROM.read(22) + 256 * EEPROM.read(23);
    counter_co2_upnot += EEPROM.read(24) + 256 * EEPROM.read(25);
    counter_co2_down += EEPROM.read(26) + 256 * EEPROM.read(27);
    counter_co2_downnot += EEPROM.read(28) + 256 * EEPROM.read(29);
    counter_ljus_there += EEPROM.read(30) + 256 * EEPROM.read(31);
    counter_ljus_not += EEPROM.read(32) + 256 * EEPROM.read(33);
    counter += EEPROM.read(34) + 256 * EEPROM.read(35);
    counter_in  += EEPROM.read(36) + 256 * EEPROM.read(37);
    log_data = 1;
  }

  if (nu_inut_tot != old_inut_tot)        //sparar värden till EEPROM
  {
    EEPROM.write(0, lowByte(EE_safecode));
    EEPROM.write(1, highByte(EE_safecode));
    EEPROM.write(2, lowByte(counter_pir_there));
    EEPROM.write(3, highByte(counter_pir_there));
    EEPROM.write(4, lowByte(counter_pir_not));
    EEPROM.write(5, highByte(counter_pir_not));
    EEPROM.write(6, lowByte(counter_ljud_there));
    EEPROM.write(7, highByte(counter_ljud_there));
    EEPROM.write(8, lowByte(counter_ljud_not));
    EEPROM.write(9, highByte(counter_ljud_not));
    EEPROM.write(10, lowByte(counter_temp_up));
    EEPROM.write(11, highByte(counter_temp_up));
    EEPROM.write(12, lowByte(counter_temp_upnot));
    EEPROM.write(13, highByte(counter_temp_upnot));
    EEPROM.write(14, lowByte(counter_temp_still));
    EEPROM.write(15, highByte(counter_temp_still));
    EEPROM.write(16, lowByte(counter_temp_stillnot));
    EEPROM.write(17, highByte(counter_temp_stillnot));
    EEPROM.write(18, lowByte(counter_temp_down));
    EEPROM.write(19, highByte(counter_temp_down));
    EEPROM.write(20, lowByte(counter_temp_downnot));
    EEPROM.write(21, highByte(counter_temp_downnot));
    EEPROM.write(22, lowByte(counter_co2_up));
    EEPROM.write(23, highByte(counter_co2_up));
    EEPROM.write(24, lowByte(counter_co2_upnot));
    EEPROM.write(25, highByte(counter_co2_upnot));
    EEPROM.write(26, lowByte(counter_co2_down));
    EEPROM.write(27, highByte(counter_co2_down));
    EEPROM.write(28, lowByte(counter_co2_downnot));
    EEPROM.write(29, highByte(counter_co2_downnot));
    EEPROM.write(30, lowByte(counter_ljus_there));
    EEPROM.write(31, highByte(counter_ljus_there));
    EEPROM.write(32, lowByte(counter_ljus_not));
    EEPROM.write(33, highByte(counter_ljus_not));
    EEPROM.write(34, lowByte(counter));
    EEPROM.write(35, highByte(counter));
    EEPROM.write(36, lowByte(counter_in));
    EEPROM.write(37, highByte(counter_in));
  }



  ljus_weight = 0;                      //påvisar inget samband då det är ljust
  if (ljus < 0.50) {                    //56.3 lägsta innan allt släkts på företaget.
    const float ljus_odds = 0.001;      //odds att den går upp om nån är där
    ljus_weight = (1 - ljus);           //ner med närvaro händer aldrig.
    if (inut_tot >= 1) {
      counter_ljus_there++;
    }
    else {
      counter_ljus_not++;
    }
  }



  if (counter < use_temp) {             //Har det gått tillräckligt med tid innan co2/temp startat ?
    counter++;
    temp_weight = 0;                    //Den gills inte innan den startat
    co2_weight = 0;                     //Den gills inte innan den startat
    temp_old = temperatur;
    co2_old = co2;
    co2_ancient = co2_old;
    ljus_old = ljus;
  }
  else {                                //Annars, beräka odds för co2, temperatur

    //beräknar data för temperatur sensorn
    temp_weight = 1;
    float tempval = (temperatur - temp_old);
    temp_old = temperatur;
    if (tempval < 0) {
      temp_odds = temp_odds_down;
      temp_weight = 0.025 + 1.95 * abs((temp_odds_down - 0.5));
      if (inut_tot >= 1) {
        counter_temp_down++;
      }
      else {
        counter_temp_downnot++;
      }
    }
    else if (tempval >= 0.01) {
      temp_odds = temp_odds_up;
      temp_weight = 0.025 + 1.95 * abs((temp_odds_up - 0.5));
      if (inut_tot >= 1) {
        counter_temp_up++;
      }
      else {
        counter_temp_upnot++;
      }
    }
    else {
      temp_odds = temp_odds_still;
      temp_weight = 0.1 + 1.8 * abs((temp_odds_still - 0.5));
      if (inut_tot >= 1) {
        counter_temp_still++;
      }
      else {
        counter_temp_stillnot++;
      }
    }

    // lyser solen på termometern ?
    float ljusval = (ljus - ljus_old);
    ljus_old = ljus;
    if (ljusval > 0.1 && ljus_old > 0.6) { //anti lys
      ljus_counter = 10;
    }
    if (ljus_counter > 0) {
      ljus_counter--;
      temp_weight = 0;
    }

    //beräknar odds för koldioxid sensorn
    float co2val = (co2 - co2_old);
    co2_weight = 1;
    if (co2val == 0) {  //om co2 blir 0
      co2val = (co2 - co2_ancient);
    }
    co2_old = co2;
    co2_ancient = co2_old;

    if (co2val < 0) {
      co2_odds = co2_odds_down;
      co2_weight = 0.025 + 1.95 * abs((co2_odds_down - 0.5));
      if (inut_tot >= 1) {
        counter_co2_down++;
      }
      else {
        counter_co2_downnot++;
      }
    }
    else {
      co2_odds = co2_odds_up;
      co2_weight = 0.025 + 1.95 * abs((co2_odds_up - 0.5));
      if (inut_tot >= 1) {
        counter_co2_up++;
      }
      else {
        counter_co2_upnot++;
      }
    }
  }

  //beräkna ljudsensor odds
  if (ljud > 730) {
    ljud_odds = ljud_odds_there;
    ljud_weight = 0.025 + 1.95 * abs((ljud_odds_there - 0.5));
    if (inut_tot >= 1) {
      counter_ljud_there++;  //räknar antalet samples som ljud har varit hög vid närvaro
    }
    else {
      counter_ljud_not++; //antalet samples som ljud varit hög vid frånvaro
    }
  }
  else {
    ljud_odds = ljud_odds_not;
    ljud_weight = 0.025 + 1.95 * abs((ljud_odds_not - 0.5));
  }

  //beräkna pir odds
  if (pir >= 1) {
    pir_weight = 0.025 + 1.95 * abs((pir_odds_not - 0.5));
    pir_odds = pir_odds_there;
    if (inut_tot >= 1) {
      counter_pir_there++;  //räknar antalet samples som ljud har varit hög vid närvaro
    }
    else {
      counter_pir_not++; //antalet samples som ljud varit hög vid frånvaro
    }
  }
  else {
    pir_weight = 0.025 + 1.95 * abs((pir_odds_not - 0.5));
    pir_odds = pir_odds_not;
  }
  total_odds_old = total_odds;
  
  //beräknar momentana oddsen
  total_odds_moment = ((pir_odds * pir_weight + temp_odds * temp_weight + co2_odds * co2_weight + ljud_odds * ljud_weight + ljus_odds * ljus_weight)
                       / (ljus_weight + ljud_weight + co2_weight + pir_weight + temp_weight));
  
  //beräknar oddsen över tid
  total_odds = (total_odds_old + total_odds_moment) / 2;

  if (total_odds > 1) {
    Serial.print("total_odds för stor:");
    Serial.println(total_odds);
    total_odds = 1;
  }
  else if (total_odds < 0) {
    total_odds = 0;
    Serial.println("total_odds negativ");
  }

  Serial.print("odds: ");
  Serial.print(total_odds);
  Serial.print("\tn:");
  Serial.print(counter);
  Serial.print("\tco2:");
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
