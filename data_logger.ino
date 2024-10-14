#include <SPI.h> 
#include <SD.h>
#include <Wire.h>
#include "RTClib.h"

RTC_DS1307 rtc;

char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
int CS = 4; 
int CLK = 2; 
int DO = 3;
int sdCS = 10;
int sw = 7;
char time_buf[16];
int outerSWpin = 5;
int innerSWpin = 6;
float dist = 0;
bool vSW = HIGH;

void setup() {
  pinMode(CS, OUTPUT); 
  pinMode(CLK, OUTPUT); 
  pinMode(DO, INPUT);
  pinMode(sw, INPUT);
  pinMode(outerSWpin, INPUT_PULLUP);
  pinMode(innerSWpin, INPUT_PULLUP);
  digitalWrite(CS, HIGH);
  digitalWrite(CLK, LOW);
  Serial.begin(9600);
  delay(2000);
  Initialize_SD();
  delay(100);
  Initialize_RTC();
  delay(100);
  //rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  //delay(100);
  }
  
void loop() {
  bool sw_state = digitalRead(sw);
  if(sw_state == HIGH){
    while(digitalRead(outerSWpin) == HIGH and digitalRead(innerSWpin) == HIGH){
      delayMicroseconds(1);
      sw_state = digitalRead(sw);
      if(sw_state == LOW){
        break;
      }
    }
    for(int i = 0; i <= 1800 ; i++){   //1800szor mer, kb. 3 kor

      //Serial.println(i);
      //delay(500);
    
    File myFile = SD.open("test.txt",FILE_WRITE);
    if (myFile){
      //for(int i = 0; i =1 ; i++){
        bool outerSW = digitalRead(outerSWpin);
        bool innerSW = digitalRead(innerSWpin);
        if(outerSW == LOW or innerSW == LOW){
          //virtual_sw(outerSW, innerSW);
          if(outerSW == LOW){
            vSW = HIGH;
            dist = 0;  //mm
            //i++;
            }
          else if(innerSW == LOW){
            vSW = LOW;
            dist = 71;  //mm
            //i++;
            }
        }
        
      if(vSW == HIGH){   //befele megy
        DateTime now = rtc.now();
        word result = ADCread();
        myFile.print(ADCread()); 
        Serial.println(result);
        myFile.print(";");
        //Serial.print("; ");
        dist = dist + 0.1;
        myFile.print(dist);
        //Serial.print(dist);
        myFile.print(";");
        //Serial.print("; ");
        sprintf(time_buf,"%02u:%02u:%02u ", now.hour(),now.minute(),now.second());
        int h = now.hour();
        int m = now.minute();
        int s = now.second();
        myFile.print(h);
        myFile.print(":");
        myFile.print(m);
        myFile.print(":");
        myFile.println(s);
        //Serial.println(time_buf);
        delay(1);
        //myFile.close();
        }
        
      else if(vSW == LOW){   //kifele megy
        DateTime now = rtc.now();
        word result = ADCread();
        myFile.print(ADCread());
        Serial.print(result);
        myFile.print(";");
        Serial.print("; ");
        dist = dist - 0.1;
        myFile.print(dist);
        Serial.print(dist);
        myFile.print(";");
        Serial.print("; ");
        sprintf(time_buf,"%02u:%02u:%02u ", now.hour(),now.minute(),now.second());
        int h = now.hour();
        int m = now.minute();
        int s = now.second();
        
        myFile.print(h);
        myFile.print(":");
        myFile.print(m);
        myFile.print(":");
        myFile.println(s);
        Serial.println(time_buf);
        delay(1);
        //myFile.close();
        }
      myFile.close();
      delay(10);
      
    }
    }
    delay(600000);  //meresek kozott 10 perc
  }
}
/*
void virtual_sw(bool outerSW, bool innerSW) {
  if(outerSW == LOW){
    vSW = HIGH;
    dist = 0;
  }
  else if(innerSW = LOW){
    vSW = LOW;
    dist = 7;
  }
  
}*/

word ADCread() {
  word _word = 0; 
  digitalWrite(CS, LOW); 
  delayMicroseconds(2); 
  digitalWrite(CLK, HIGH); 
  delayMicroseconds(2); 
  digitalWrite(CS, HIGH); 
  delayMicroseconds(0.1); 
  digitalWrite(CS, LOW); 
  delayMicroseconds(4);  //Wake-Up time 
  digitalWrite(CLK, LOW); 
  delayMicroseconds(8); //Conversion time 
  for (int j=2; j>=0; j--)
    { digitalWrite(CLK, HIGH); 
    delayMicroseconds(2); 
    digitalWrite(CLK, LOW); 
    delayMicroseconds(2); } 
  for (int i=11; i>=0; i--) { 
    digitalWrite(CLK, HIGH); 
    delayMicroseconds(2); 
    digitalWrite(CLK, LOW); 
    delayMicroseconds(2); 
    if (digitalRead(DO)) { 
      _word |= (1 << i); 
      } 
    } 
  digitalWrite(CS, HIGH); 
  return _word; 
  }




void Initialize_SD() {
  while (!Serial) 
    { //; 
    }
    Serial.println("SD kártya felismerése...");
  if (!SD.begin(sdCS)) {
    Serial.println("SD kártya felismerése nem sikerült!");
    while (1);
    }
  Serial.println("SD kártya használatra kész");
}



void Initialize_RTC() {
  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
    }
  if (!rtc.isrunning()) {
    Serial.println("RTC lost power, lets set the time!");
  
    // Comment out below lines once you set the date & time.
    // Following line sets the RTC to the date & time this sketch was compiled
    //rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  
    // Following line sets the RTC with an explicit date & time
    // for example to set January 27 2017 at 12:56 you would call:
    //rtc.adjust(DateTime(2021, 5, 10, 13, 14, 0));
    }
}
