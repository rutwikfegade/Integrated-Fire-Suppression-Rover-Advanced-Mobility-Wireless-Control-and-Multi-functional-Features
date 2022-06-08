#include "RF24.h"

RF24 radio(7, 8);
#define pwm1          10
#define pwm2          9
#define pwm3          3
#define pwm4          2   
#define thrower       6
#define water         5
#define sound         4
byte addresses[][6] = {"0001","0002"};
const int led=6;
void configureRadio() {
 
  radio.begin();
  radio.setPALevel(RF24_PA_LOW);

  radio.openWritingPipe(addresses[0]);
  radio.openReadingPipe(1,addresses[1]);
  radio.startListening();
  
}
struct Datatoreceive {
 int valuex;
 int valuey;
 int w_p;
 int s_p;
 int t_p;
};
uint32_t configTimer =  millis();

void setup() {
  Serial.begin(9600);
  configureRadio();
  pinMode(led,OUTPUT);
  pinMode(pwm1,   OUTPUT);
  pinMode(pwm2,   OUTPUT);
  pinMode(pwm3,   OUTPUT);
  pinMode(pwm4,   OUTPUT);
}

void loop()
{ 
  Datatoreceive Data;
  if(radio.failureDetected) {
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
if (radio.available()) {
      uint32_t failTimer = millis();
      int value=0;
      while (radio.available())                                
      {
        if (millis() - failTimer > 500) {
          Serial.println("Radio available failure detected");
          radio.failureDetected = true;
          break;
        }
          radio.read(&Data, sizeof(Data));          
      }

  if(Data.valuey < 490){
    digitalWrite(pwm1, HIGH);
      digitalWrite(pwm2,0);
      digitalWrite(pwm4, 0);
      digitalWrite(pwm3, HIGH);
    
  }
  else
    if(Data.valuey > 500){
       digitalWrite(pwm1, 0);
      digitalWrite(pwm2,HIGH);
      digitalWrite(pwm4, HIGH);
      digitalWrite(pwm3, 0);
    }
 
  else  if(Data.valuex < 480){
    digitalWrite(pwm2, 0);
    digitalWrite(pwm1, (HIGH));
    digitalWrite(pwm4, 0);
    digitalWrite(pwm3, 0);
  }
  else
    if(Data.valuex > 500){
      digitalWrite(pwm1, 0);
      digitalWrite(pwm2,0);
      digitalWrite(pwm4, 0);
      digitalWrite(pwm3, (HIGH));
    }
    
    else if(Data.t_p==1)
    {
      digitalWrite(thrower,HIGH);
        delay(5000);
       digitalWrite(thrower,LOW);
    }
    else if (Data.w_p==1)
    {
      digitalWrite(water,HIGH);
    }
    
    else if(Data.s_p==1)
    {
      digitalWrite(sound,HIGH);
    }
    else
    {
      digitalWrite(water,LOW);
      digitalWrite(sound,LOW);
      digitalWrite(pwm1, 0);
      digitalWrite(pwm2, 0);  
      digitalWrite(pwm3, 0);
      digitalWrite(pwm4, 0);
    }

   radio.stopListening();                           
      radio.write(&value, sizeof(int));           
      radio.startListening();
      
}
}
