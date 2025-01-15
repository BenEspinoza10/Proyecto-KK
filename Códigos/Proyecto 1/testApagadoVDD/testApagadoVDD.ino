#include "LowPower.h"

void wakeUp()
{
    // Just a handler for the pin interrupt.
}
void setup() {
  // put your setup code here, to run once:
  pinMode(2,INPUT);
  Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("Apagado");   
  attachInterrupt(digitalPinToInterrupt(2), wakeUp, CHANGE);
  pinMode(4,INPUT);
  LowPower.powerDown(SLEEP_FOREVER, ADC_OFF, BOD_OFF);
  detachInterrupt(digitalPinToInterrupt(2)); 
  pinMode(4,OUTPUT);
  digitalWrite(4,LOW);
  Serial.println("movimiento activado");  
  delay(5000);
}
