#include "RF24.h"

RF24 radio(7, 8);

byte addresses[][6] = {"0001","0002"};
const int led=6;
const int joyx=A0;
const int joyy=A1;
const int water=5;
const int sound=4;
const int thrower=3;
bool buttonstate=LOW;
struct Datatosend {
 int valuex;
 int valuey;
 int w_p;
 int s_p;
 int t_p;
};
void configureRadio() {
 
  radio.begin();
  radio.setPALevel(RF24_PA_LOW);

  radio.openWritingPipe(addresses[1]);
  radio.openReadingPipe(1,addresses[0]);
  radio.startListening();
  
}

uint32_t configTimer =  millis();

void setup() {
  Serial.begin(9600);
  pinMode(led,OUTPUT);
  pinMode(joyx,INPUT);
  pinMode(joyy,INPUT);
  pinMode(water,INPUT);
  pinMode(sound,INPUT);
  pinMode(thrower,INPUT);
  configureRadio();
}

void loop() {
  Datatosend Data;
  Data.valuex=analogRead(joyx);
  Data.valuey=analogRead(joyy);
  Data.w_p=debounce(buttonstate,water);
  Data.s_p=debounce(buttonstate,sound);
  Data.t_p=debounce(buttonstate,thrower);
  digitalWrite(led,HIGH);
  if (radio.failureDetected) {
    radio.failureDetected = false;
    delay(250);
    Serial.println("Radio failure detected, restarting radio");
    configureRadio();
  }

  if (millis() - configTimer > 5000) {
    configTimer = millis();
    if (radio.getDataRate() != RF24_1MBPS) {
      radio.failureDetected = true;
      Serial.print("Radio configuration error detected");
    }
  }

  radio.stopListening(); 

    int value=5;                     
    if (!radio.write(&Data, sizeof(Data))) 
    {
      Serial.println("failed");
      
    }
    radio.startListening();                                   
 
    unsigned long started_waiting_at = micros();              
    bool timeout = false;                                     
 
    while (!radio.available())                                

    {
      if (micros() - started_waiting_at > 200000 )             
      {
        timeout = true;
        break;
      }
    }
 
    if (timeout) {
      digitalWrite(led,LOW);
      Serial.println("Failed, response timed out.");
    } 
    else 
    {
  uint32_t failTimer = millis();
      while (radio.available())                               
      {
        if (millis() - failTimer > 250) 
        {
          radio.failureDetected = true;
          Serial.println("Radio available failure detected");
          break;
        }
        
      }
}
delay(1000);
}

boolean debounce(boolean state,int buttonpin)
{
  boolean stateNow = digitalRead(buttonpin);
  if(state!=stateNow)
  {
    delay(20);
    stateNow = digitalRead(buttonpin);
  }
  return stateNow;
  
}
