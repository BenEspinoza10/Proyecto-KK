#include "headers.h"
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  while (!Serial)
    ;
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(IN_INTERRUPT, INPUT);
  pinMode(PUSH_BUTTON, INPUT_PULLUP);
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_BLUE, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(OUT_DISABLE_POWER,OUTPUT); //Se pone el pin en escritura
  digitalWrite(OUT_DISABLE_POWER,LOW);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.print("Vibracion: ");
  Serial.print(digitalRead(IN_INTERRUPT));
  Serial.print("`lineas: ");
  Serial.print(analogRead(IN_LINE));
  Serial.print(" distancia: ");
  Serial.print(analogRead(IN_DIAMETER));
  Serial.print("botón: ");
  Serial.println(digitalRead(PUSH_BUTTON));
}
