#include <Arduino.h>
#include "BasicStepperDriver.h"
#include "MultiDriver.h"
#include "SyncDriver.h"


//#define BLYNK_PRINT Serial
// Motor steps per revolution. Most steppers are 200 steps or 1.8 degrees/step
#define MOTOR_STEPS 200 //500 volt
#define RPM 240

// Since microstepping is set externally, make sure this matches the selected mode
// If it doesn't, the motor will move at a different RPM than chosen
// 1=full step, 2=half step etc.
#define MICROSTEPS 1

#define DIR 3 //Mindenkepp kell ide valami
#define STEP_CW 8
#define STEP_CWW 9
#define SLEEP_CW 8
#define SLEEP_CWW 9


#include <SoftwareSerial.h>
SoftwareSerial SerialBLE(10, 11); // RX, TX

//#include <BlynkSimpleSerialBLE.h>

//int cw_pin = 5;
//int cww_pin = 6;
int ac_switch = 7;
int valueStatus = 0;

BasicStepperDriver stepper_CW(MOTOR_STEPS, DIR, STEP_CW, SLEEP_CW);
BasicStepperDriver stepper_CWW(MOTOR_STEPS, DIR, STEP_CWW, SLEEP_CWW);

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
//char auth[] = "A012_2j8YOAzkSg4fSjsNEOgSJjqZYvK";
//BlynkTimer timer;

// This function will be called every time Slider Widget
// in Blynk app writes values to the Virtual Pin 1
/*BLYNK_WRITE(V0)
{
  valueStatus = param.asInt(); // Get value as integer
  Serial.println(valueStatus);
  if(valueStatus == 1){
    digitalWrite(ac_switch, HIGH);
  }
  else if(valueStatus == 0){
    digitalWrite(ac_switch, LOW);
  }
}*/

void setup()
{
  // Debug console
  Serial.begin(9600);
  pinMode(4, INPUT_PULLUP);
  
  //SerialBLE.begin(9600);
  //Blynk.begin(SerialBLE, auth);
  //timer.setInterval(10, stepperControl); // updates stepper control every 10ms

  //Serial.println("Waiting for connections...");
  stepper_CW.begin(RPM, MICROSTEPS);
  stepper_CWW.begin(RPM, MICROSTEPS);
  stepper_CW.setEnableActiveState(LOW);
  stepper_CWW.setEnableActiveState(LOW);
  digitalWrite(ac_switch, LOW);
  Serial.println("Led low");
}

void loop()
{
  //Blynk.run();
  //timer.run();
  valueStatus = digitalRead(4);
  Serial.println(valueStatus);
  stepperControl();
  
}

void stepperControl(){

  if(valueStatus == 0){
    digitalWrite(ac_switch, HIGH);
    stepper_CWW.move(33300); //21600  //35000  //32900
    stepper_CWW.disable();
    delay(100);
    stepper_CW.move(33300);
    stepper_CW.disable();
    delay(100);
  }

  else if(valueStatus == 1){
    digitalWrite(ac_switch, LOW);
    stepper_CWW.disable();
    stepper_CWW.disable();
  }
  
}
