#ifndef ljudsensor_h
#define ljudsensor_h
#include "arduino.h"

int MIC_PIN = A5;                               //Deklarerar ingång för tempsensor

float microphone_minmaxljud = 1023;    //variabel för att hitta tyst.

float microphone(int trackmax = 10000,bool no_write = 1)
{
  const float microphone_tyst = 585;                       //högsta uppmätta värdet, då rummet är tyst
  const float microphone_prat = 600;                       //uppmätt värde, för tal i rummet (det viktiga är prat/tyst)

  static float microphone_maxljud    = 0;                //sätt till litet
  static float microphone_minimum    = 1023;               //sätt till stort
  static unsigned int count_ja = 0;
  static float tempfloat = 0;
  int read5 = 0;
  static unsigned long int track=0;                                      //variable to keep track
  float microphone_numax=0;                                //declare and initialize microphone_minmaxljud as zero
  while(1)
  {
    read5 = analogRead(MIC_PIN);
    if (read5>microphone_maxljud) microphone_maxljud=read5;
    if (read5<microphone_minimum) microphone_minimum=read5;
    if (read5>microphone_maxljud) microphone_maxljud=read5;
    if (read5>microphone_numax)   microphone_numax=read5;
    track++;//increase track by 1 after every iteration
    if (track==trackmax){
      count_ja++;
      track=0; //set back track to zero
      if (microphone_numax<microphone_minmaxljud && microphone_numax > (microphone_maxljud*0.55) 
        && microphone_minimum<microphone_minmaxljud) microphone_minmaxljud=microphone_numax; 
      return microphone_numax;
      /*if (microphone_numax < microphone_minmaxljud*(microphone_prat/microphone_tyst)){
            if(no_write == 0)
            {
              Serial.print("Ljudsensor:\t");
              Serial.print("Nej (");
              Serial.print(microphone_numax);
              Serial.print(")\tmax: ");
              Serial.print(microphone_maxljud);
              Serial.print(" minmax: ");
              Serial.println(microphone_minmaxljud);
            }
            count_ja = 0;
            return (((microphone_numax-microphone_minmaxljud/2)/(microphone_maxljud-microphone_minmaxljud/2))/4);
      }
      else{
            if(no_write == 0){
            Serial.print("Ljudsensor:\t");
            Serial.print("Ja (");
            Serial.print(microphone_numax);
            Serial.print(")\tmax: ");
            Serial.print(microphone_maxljud);
            Serial.print(" minmax: ");
            Serial.println(microphone_minmaxljud);
            }
            if (count_ja >= 7){
              microphone_minmaxljud += 0.5;          //för att justerar för statiskt bakrunds bös
              if (count_ja >= 29){
                microphone_minmaxljud += 2.5;          //för att justerar för statiskt bakrunds bös
              }
            }
            return ((microphone_numax-microphone_minmaxljud/2)/(microphone_maxljud-microphone_minmaxljud/2));
      }*/
    }
  }
}
#endif
