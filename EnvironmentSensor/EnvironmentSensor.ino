#include "ljudsensor.h"       //microphone();
#include "temp2sensor.h"      //temperature();
#include "fukt4sensor.h"      //moist();
#include "Gas_sensor.h"       //gas();
#include "pirsensor.h"        //pir();
//#include "voltage_check.h"    //volt();
#include "loginut.h"          //inut();
#include "ljussensor.h"       //light();
#include <SoftwareSerial.h>
#include "ESP8266.h"          //esp_8266(float temperature = 0, float co2 = 0, float ljud = 0, float fukt = 0,float ljus = 0,int pir = 0,int antal = 0);

float temptemp2 = 0;
float tempgas2 = 0;
float temptemp = 0;
float tempgas = 0;
int temppir = 0;
float tempmoist = 0;
float templight = 0;
const int antal_odds = 3;

void setup() {
  ser.begin(115200);                       //Sätt baud rate (bit/s)
  ser.println("AT+RST");
  Serial.println("Initializing pins");
  pinMode(A1, INPUT);                       //A1, ljussensor
  pinMode(IN_PIN, OUTPUT);                  //A2, fukt
  pinMode(OUT_PIN, OUTPUT);                 //A3, fukt
  pinMode(MIC_PIN, INPUT);                  //A4, mikrofon
  pinMode(GAS_PIN, INPUT);                  //A5, co2
  pinMode(PIR_PIN, INPUT);                  //D2, PIR interrupt
  pinMode(3, INPUT);                        //D3, räknare interrupt
  pinMode(4, INPUT);                        //D4, räknare in
  pinMode(5, INPUT);                        //D5, räknare ut
  pinMode(PWM_PIN, INPUT);                  //D13, temp
  
  /*Serial.println("Initializing microphone");
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
  Serial.println("Initializing interrupts");*/
  attachInterrupt(digitalPinToInterrupt(PIR_PIN), pir, RISING);
  attachInterrupt(digitalPinToInterrupt(3), inut, RISING);   //initializing inut
  Serial.println("SETUP COMPLETE");
  Serial.println(" ");
  Serial.println("temp\ttempK\tCO2ppm\tKppm\tljudnu\tljudmin\tfukt\tfuktK\tljus\tpir\tantal");
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
  tempmoist = moist(0);
  Serial.print("\t");
  templight = light(); 
  Serial.print(templight);
  if (pirhigh >= 1) {
    Serial.print("\t");
    Serial.print(pirhigh);
    Serial.print("\t");
    temppir = pirhigh;
    pirhigh = 0;
  }
  else if( PIR_PIN == 1){
    Serial.print("\t1\t");
    temppir = 1;
  }
  else {
    temppir = 0;
    Serial.print("\t0\t");
  }
  Serial.println(inut_tot);
  esp_8266(temptemp, tempgas, microphone_minmaxljud, tempmoist,templight,temppir,inut_tot);
}

