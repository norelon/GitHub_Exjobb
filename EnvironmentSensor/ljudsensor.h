#ifndef ljudsensor_h
#define ljudsensor_h
#include "arduino.h"

byte MIC_PIN = 4;                                         //Deklarerar ingång för tempsensor

float microphone_minmaxljud = 1023;                       //Variabel för att hitta tyst
float microphone_Xe = 600;                                   //Startvärde
float microphone(int trackmax = 10000, bool no_write = 1)
{
  static float variance = 82.16035311;                      //Uppmätt värde efter 200 samplingar utan Kalman
  static float varianceProcess = 1e-4;                      //Snabbhet i systemet
  static float Pc = 0.0;
  static float G = 0.0;
  static float P = 1.0;
  static float Xp = 0.0;
  static float Zp = 0.0;

  const float microphone_tyst = 570;                      //Högsta uppmätta värdet, då rummet är tyst
  const float microphone_prat = 600;                      //Uppmätt värde, för tal i rummet (det viktiga är prat/tyst)
  static float microphone_maxljud    = 0;                 //Sätt till litet
  static float microphone_minimum    = 1023;              //Sätt till stort
  static unsigned int count_ja = 0;
  static float tempfloat = 0;

  int read5 = 0;
  static unsigned long int track = 0;                     //Variabel för track
  float microphone_numax = 0;                             //Deklarera microphone_minmaxljud och sätt till 0

  while (1)
  {
    read5 = analogRead(MIC_PIN);
    
    //Ta fram max- respektive min värden
    if (read5 > microphone_maxljud) microphone_maxljud = read5;
    if (read5 < microphone_minimum) microphone_minimum = read5;
    if (read5 > microphone_maxljud) microphone_maxljud = read5;
    if (read5 > microphone_numax)   microphone_numax = read5;
    
    //räkna upp mot samplings periodens slut
    track++;
    if (track == trackmax) {
      count_ja++;
      track = 0; //set back track to zero
      if (microphone_numax < microphone_minmaxljud && microphone_numax > (microphone_maxljud * 0.55) 
          && microphone_minimum < microphone_minmaxljud) microphone_minmaxljud = microphone_numax;  //väljer nuvarande toppvärde
      
      //Kalmanprocess
      Pc = P + varianceProcess;
      G = Pc / (Pc + variance);
      P = (1 - G) * Pc;
      Xp = microphone_Xe;
      Zp = Xp;
      microphone_Xe = G * (microphone_numax - Zp) + Xp;
      
      return microphone_numax;
    }
  }
}
#endif
