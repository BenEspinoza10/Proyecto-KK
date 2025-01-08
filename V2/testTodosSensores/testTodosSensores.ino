#include "headers.h"
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  while (!Serial)
    ;  
  pinMode(HALL, INPUT);
  pinMode(OPEN_SENSOR, INPUT);    
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.print("Efecto hall: ");
  Serial.print(digitalRead(HALL));
  Serial.print(" Sensor apertura: ");
  Serial.println(digitalRead(OPEN_SENSOR));  
}
