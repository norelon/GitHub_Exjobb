#include "ljudsensor.h"       //microphone();
#include "temp2sensor.h"      //temperature();
#include "fukt4sensor.h"      //moist();
#include "Gas_sensor.h"       //gas();
#include "pirsensor.h"        //pir();
//#include "voltage_check.h"    //volt();
#include "loginut.h"          //inut();
#include "ljussensor.h"       //light();
#include <SoftwareSerial.h>
#include "odds.h"
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
  Serial.begin(115200);                   //funkar inte med ser.begin() serial tar över?
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
  pinMode(ESP_RST, OUTPUT);                  //D8, esp reset pin
  pinMode(PWM_PIN, INPUT);                  //D13, temp
  Serial.println("Initializing ESP8266");
  ser.begin(115200);                        //Sätt baud rate (bit/s)
  ser.println("AT+RST");                    //ESPsoftware reset
  Serial.println("Initializing microphone");
  /*Serial.print("Loading[..");
  microphone(20000, 1);
  Serial.print("....");
  microphone(10000, 1);
  Serial.print("..");
  microphone(10000, 1);
  Serial.println("..]");
  Serial.println("Initializing Temperature sensor");
  Serial.print("Loading[");
  const int antali = 10;
  for (int i = 0; i < antali; i++) {
    if (i % (antali/10) == 0) Serial.print(".");
    temperature(385, 1);
  }
  Serial.println("]");*/
  Serial.println("Initializing interrupts");
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
  const int antal_loop = 5;
  tempmic=0;
  for (int i = 0; i < antal_loop; i++) {
    if(i == 3) esp_reset();     //för stabilitet på internet uppkopplingen placerades här för att få en effektiv delay.
    //tempmic = microphone(10000, 1);
    //if (tempmic > micodds) micodds = tempmic; // letar efter peakvärden, då händelser är mest intressanta
    //gas(1);
    //moist(1);
    temperature(385, 1);
  }
  //micodds = tempmic/(antal_loop);
  temptemp = temperature(385, 0);
  tempgas = gas(0);
  Serial.print("\t");
  Serial.print(micodds);
  Serial.print("\t");
  Serial.print(microphone_Xe);
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
  else if ( PIR_PIN == 1) {
    Serial.print("\t1\t");
    temppir = 1;
  }
  else {
    temppir = 0;
    Serial.print("\t0\t");
  }
  Serial.println(inut_tot);
  odds(temppir,temptemp);
  esp_8266(temptemp, tempgas, micodds, tempmoist, templight, temppir, inut_tot);
}

