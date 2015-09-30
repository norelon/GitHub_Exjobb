#include "ljudsensor.h"   //microphone();
#include "temp2sensor.h"  //temperature();
#include "fukt4sensor.h"  //moist();
#include "Gas_sensor.h"   //gas();
#include "pirsensor.h"    //pir();
#include "voltage_check.h"    //volt();

  float temptemp2 = 0;
  float tempgas2 = 0; 
  float temptemp = 0;
  float tempgas = 0; 
  const int antal_odds = 3;
  
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600); //set baud rate
  Serial.println("Initializing pins");
  pinMode(PWM_PIN, INPUT);        //13
  pinMode(0, OUTPUT);             //A0, för att se volt drop
  pinMode(1, OUTPUT);             //A1, för att se volt drop
  pinMode(IN_PIN, OUTPUT);        //A2, för temperatur
  pinMode(OUT_PIN, OUTPUT);       //A3, för temperatur
  pinMode(MIC_PIN, INPUT);        //A4
  pinMode(5, INPUT);              //A5 gas sensor
  pinMode(LED_PIN,OUTPUT);        //Sätter digital pin 4 till utgång för LED-lampa
  pinMode(PIR_PIN,INPUT);         //Sätter digital pin 2 till ingång
  
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
  interrupt_init();
  Serial.println("SETUP COMPLETE");
  Serial.println(" ");
}

void loop() {
  int temp = 0;
  float tempmic = 0;
  float micodds = 0;
  // put your main code here, to run repeatedly:
  Serial.println(" ");
  volt();
  temptemp2 = temptemp;
  tempgas2 = tempgas;
  for(int i = 0;i < 10;i++){
    if (i%2 == 0){
      tempmic = microphone(10000,1);
      if (tempmic>micodds) micodds=tempmic;
      gas(1);
      moist(1);
    }   
    temperature(385,1);
  }
  temptemp = temperature(385,1);
  tempgas = gas(0);
  Serial.print("Ljudsensor:\t");
  if(micodds>0.5) Serial.print("JA\t(");
  else  Serial.print("NEJ\t(");
  Serial.print(micodds);
  Serial.println(")");
  Serial.print("Temperatur:\t");
  Serial.print(temptemp);
  Serial.println(" Celsius");
  moist(0);
  if (tempmic >= 0.95) print_odds(tempmic);
  else if(pirhigh >= 1 || PIR_PIN == 1)
  { 
    pirhigh = 0;
    print_odds(0.95);
  }
  else if (micodds > 0.25) print_odds(micodds); 
  else if (temptemp2 < temptemp && (tempgas2/10) < (tempgas/10)) print_odds(0.25);
  else print_odds(micodds);
}

void print_odds(float nuodds)
{
    static float odds[antal_odds];
    static float odds_sum = 0;
    float tempfloat = 0;
    odds[antal_odds] = nuodds;
    for(int i = 0; i < antal_odds; i++)
    {
       odds[i] = odds[i+1];
       odds_sum = odds_sum + odds[i]; 
       Serial.print(odds[i]);
       Serial.print(" "); 
    }
    tempfloat = antal_odds;
    Serial.print(" ;");
    Serial.println(odds_sum/tempfloat);
    odds_sum = 0;
}

