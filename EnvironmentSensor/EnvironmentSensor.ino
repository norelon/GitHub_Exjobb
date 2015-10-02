#include "ljudsensor.h"       //microphone();
#include "temp2sensor.h"      //temperature();
#include "fukt4sensor.h"      //moist();
#include "Gas_sensor.h"       //gas();
#include "pirsensor.h"        //pir();
#include "voltage_check.h"    //volt();
#include "loginut.h"          //inut();

float temptemp2 = 0;
float tempgas2 = 0;
float temptemp = 0;
float tempgas = 0;
const int antal_odds = 3;

void setup() {
  Serial.begin(9600);                       //Sätt baud rate (bit/s)
  Serial.println("Initializing pins");
  pinMode(A0, OUTPUT);                      //A0, volt drop
  pinMode(A1, OUTPUT);                      //A1, volt drop
  pinMode(IN_PIN, OUTPUT);                  //A2, fukt
  pinMode(OUT_PIN, OUTPUT);                 //A3, fukt
  pinMode(MIC_PIN, INPUT);                  //A4, mikrofon
  pinMode(GAS_PIN, INPUT);                  //A5, co2
  pinMode(PIR_PIN, INPUT);                  //D2, PIR interrupt
  pinMode(3, INPUT);                        //D3, räknare interrupt
  pinMode(4, INPUT);                        //D4, räknare in
  pinMode(5, INPUT);                        //D5, räknare ut
  pinMode(PWM_PIN, INPUT);                  //D13, temp
  
  Serial.println("Initializing microphone");
  Serial.print("Loading[");
  microphone(20000,1);
  for(int i = 0;i < 10;i++){
    Serial.print(".");
    microphone(10000,1);
  }
  microphone(5000,1);
  microphone(5000,1);
  Serial.println("]");
  Serial.println("Initializing Temperature sensor");
  Serial.print("Loading[");
  for(int i = 0;i < 50;i++){
    if(i%5==0) Serial.print(".");
    temperature(385,1);
  }
  Serial.println("]");
  Serial.println("Initializing interrupts");
  attachInterrupt(digitalPinToInterrupt(PIR_PIN), pir, RISING);
  attachInterrupt(digitalPinToInterrupt(3), inut, RISING);   //initializing inut
  Serial.println("SETUP COMPLETE");
  Serial.println(" ");
  Serial.println("temp\ttempK\tCO2ppm\tKppm\tljudnu\tljudmin\tfukt\tfuktK\tpir\tantal");
}

void loop() {
  int temp = 0;
  float tempmic = 0;
  float micodds = 0;
  // put your main code here, to run repeatedly:
  temptemp2 = temptemp;
  tempgas2 = tempgas;
  for (int i = 0; i < 10; i++) {
    if (i % 2 == 0) {
      tempmic = microphone(10000, 1);
      if (tempmic > micodds) micodds = tempmic;
    }
    gas(1);
    moist(1);
    temperature(385, 1);
  }
  temptemp = temperature(385, 0);
  tempgas = gas(0);
  Serial.print("\t");
  Serial.print(micodds);
  Serial.print("\t");
  Serial.print(microphone_minmaxljud);
  moist(0);
  if (pirhigh >= 1) {
    Serial.print("\t");
    Serial.print(pirhigh);
    Serial.print("\t");
    pirhigh = 0;
  }
  else if( PIR_PIN == 1){
  Serial.print("\t1\t");
  }
  else {
    Serial.print("\t0\t");
  }
  Serial.println(inut_tot);
  /*if (tempmic >= 0.95) print_odds(tempmic);
  else if(pirhigh >= 1 || PIR_PIN == 1)
  {
    pirhigh = 0;
    print_odds(0.95);
  }
  else if (micodds > 0.25) print_odds(micodds);
  else if (temptemp2 < temptemp && (tempgas2/10) < (tempgas/10)) print_odds(0.25);
  else print_odds(micodds);*/
}

void print_odds(float nuodds)
{
  static float odds[antal_odds];
  static float odds_sum = 0;
  float tempfloat = 0;
  odds[antal_odds] = nuodds;
  for (int i = 0; i < antal_odds; i++)
  {
    odds[i] = odds[i + 1];
    odds_sum = odds_sum + odds[i];
    Serial.print(odds[i]);
    Serial.print(" ");
  }
  tempfloat = antal_odds;
  Serial.print(" ;");
  Serial.println(odds_sum / tempfloat);
  odds_sum = 0;
}

