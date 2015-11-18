#include "ljudsensor.h"       //microphone();
#include "temp2sensor.h"      //temperature();
#include "Gas_sensor.h"       //gas();
#include "pirsensor.h"        //pir();
#include "loginut.h"          //inut();
#include "ljussensor.h"       //light();
#include <SoftwareSerial.h>
#include "odds.h"
#include "ESP8266.h"          //esp_8266(float temperature = 0, float co2 = 0, float ljud = 0, float fukt = 0,float ljus = 0,int pir = 0,int antal = 0);
#include "Sleep_n0m1.h"       //external sleep library.

float temptemp = 0;
float tempgas = 0;
int temppir = 0;
float tempmoist = 0;
float templight = 0;
const int antal_odds = 3;

Sleep sleep;                  //sleep class

void setup() {
  //Serial.begin(115200);                   //funkar inte med ser.begin() serial tar över?
  Serial.println("Initializing pins");
  pinMode(A1, INPUT);                       //A1, ljussensor
  pinMode(MIC_PIN, INPUT);                  //A4, mikrofon
  pinMode(GAS_PIN, INPUT);                  //A5, co2
  pinMode(PIR_PIN, INPUT);                  //D2, PIR interrupt
  pinMode(3, INPUT);                        //D3, räknare interrupt
  pinMode(4, INPUT);                        //D4, räknare in
  pinMode(5, INPUT);                        //D5, räknare ut
  pinMode(ESP_RST, OUTPUT);                 //D8, esp reset pin
  pinMode(PWM_PIN, INPUT);                  //D13, temp
  Serial.println("Initializing ESP8266");
  ser.begin(115200);                        //Sätt baud rate (bit/s) för kommunikation med ESP8266
  ser.println("AT+RST");                    //ESPsoftware reset
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
  float chance    = 0;
  static unsigned int sleep_counter = 0;
 
  const int antal_loop = 10; //bör vara större än 7
  tempmic=0;
  for (int i = 0; i < antal_loop; i++) {              //kör ett flertalgånger för att få stabilare värden med kalman.
    if(i == 3) digitalWrite(ESP_RST, LOW);            //spara stöm genom att stänga av ESP8266, för stabilitet på internet uppkopplingen placerades den här för att få en effektiv delay.
    if(i == antal_loop-5) digitalWrite(ESP_RST, HIGH);//startar upp ESP8266 lite i förväg för att försäkra att den hinner starta.
    tempmic = microphone(10000, 1);
    if (tempmic > micodds) micodds = tempmic;         // sparar största peakvärdet
    gas(1);
    temperature(385, 1);
  }
  temptemp = temperature(385, 0);
  tempgas = gas(0);
  Serial.print("\t");
  Serial.print(micodds);
  Serial.print("\t");
  Serial.print(microphone_Xe);
  Serial.print("\tn/a\tn/a");
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
  chance = odds(temppir,temptemp,tempgas,micodds,templight); //beräknar sannolikheten att någon är i rummet
  
  esp_8266(temptemp, tempgas, micodds, tempmoist, templight, temppir, inut_tot, chance);  // skickar data till thingspeak
  
  if(chance < 0.3 && temppir == 0) sleep_counter++;   //under ~30% sannolikhet är ungefär minsta möjliga odds. 
  else sleep_counter = 0;
  if(sleep_counter >= 5){                             //går i sleep mode tills interrupt påträffas, om oddsen varit låg länge
      Serial.println("--Sleep mode--");
      digitalWrite(ESP_RST, LOW);
      delay(100);
      sleep_counter = 0;
      sleep.pwrDownMode();                            //set sleep mode
      sleep.sleepInterrupt(1,RISING);
      digitalWrite(ESP_RST, HIGH);
      Serial.println("--Normal mode--");
  }
}

