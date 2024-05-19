#include"headers.h"
void setup_general(){
  Serial.begin(115200);
  while (!Serial)
    ;
  pinMode(IN_LINE, INPUT);
  pinMode(LED_BUILTIN, OUTPUT);
}
void blink_led_builin(int time_hi, int hm_times) {
  for (int i = 0; i < hm_times; i++) {
    digitalWrite(LED_BUILTIN, HIGH);  // turn the LED on (HIGH is the voltage level)
    delay(time_hi);                   // wait for a second
    digitalWrite(LED_BUILTIN, LOW);   // turn the LED off by making the voltage LOW
    delay(time_hi);
  }  // wait for a second
}

void blink_led(int time_hi, int hm_times, int digitalPin) {
  for (int i = 0; i < hm_times; i++) {
    digitalWrite(digitalPin, HIGH);  // turn the LED on (HIGH is the voltage level)
    delay(time_hi);                   // wait for a second
    digitalWrite(digitalPin, LOW);   // turn the LED off by making the voltage LOW
    delay(time_hi);
  }  // wait for a second
}

void print_distancias(){
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