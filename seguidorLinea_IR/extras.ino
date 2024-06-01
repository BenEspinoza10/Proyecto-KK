#include "headers.h"
void setup_general() {
  Serial.begin(115200);
  while (!Serial)
    ;
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(IN_INTERRUPT, INPUT);
  pinMode(PUSH_BUTTON, INPUT_PULLUP);
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_BLUE, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
}
void blink_led_builin(int time_hi, int hm_times) {
  for (int i = 0; i < hm_times; i++) {
    digitalWrite(LED_BUILTIN, HIGH);  // turn the LED on (HIGH is the voltage level)
    delay(time_hi);                   // wait for a second
    digitalWrite(LED_BUILTIN, LOW);   // turn the LED off by making the voltage LOW
    delay(time_hi);
  }  // wait for a second
}

void led_red_on() {
  digitalWrite(LED_RED,HIGH);
  digitalWrite(LED_BLUE,LOW);
  digitalWrite(LED_GREEN,LOW);
}

void led_red_on() {
  digitalWrite(LED_RED,LOW);
  digitalWrite(LED_BLUE,LOW);
  digitalWrite(LED_GREEN,HIGH);
}

void led_off() {
  digitalWrite(LED_RED,LOW);
  digitalWrite(LED_BLUE,LOW);
  digitalWrite(LED_GREEN,LOW);
}

void print_distancias() {
  radio_temp = (1 - MeasureAnalogN(SAMPLES, IN_DIAMETER));
  sensorRadio = radio_max * radio_temp;
  Serial.print("valor real: ");
  Serial.print(analogRead(IN_DIAMETER));
  Serial.print("valor filtrado: ");
  Serial.print(MeasureAnalogN(SAMPLES, IN_DIAMETER));
  Serial.print("radio ratio temp: ");
  Serial.print(radio_temp);
  Serial.print("radio calculado: ");
  Serial.println(sensorRadio);
}

void print_temporal_tirada() {
  Serial.println("%rollo; radio; giro ;  cms ");
  Serial.print(radio_temp * 100, 0);
  Serial.print(" ; ");
  Serial.print(sensorRadio, 2);
  Serial.print(" ; ");
  Serial.print(vueltas_temp, 2);
  Serial.print(" ; ");
  Serial.println(gasto_temp, 1);
}