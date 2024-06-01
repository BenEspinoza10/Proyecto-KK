#include "LowPower.h"

void setup() {
  // put your setup code here, to run once:
  
}

void loop() {
  // put your main code here, to run repeatedly:
  pinMode(2,INPUT);
  LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);
  pinMode(2,OUTPUT);
  digitalWrite(12,LOW);
  delay(1000);
}
