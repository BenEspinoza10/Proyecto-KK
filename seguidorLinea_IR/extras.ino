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


void led_rgb_tOn(int rr ,int gg, int bb, int ms) {
  int r=rr;
  int g=gg;
  int b=bb;
  int milis=ms;
  if (r>255) r=255;
  if (g>255) g=255;
  if (b>255) b=255;

  int m=50;
  int t;
  while (milis>0){
      led_red_on();
      t=int(r/m);
      delay(t);
      milis=milis-t;
      led_green_on();
      t=int(g/m);
      delay(t);
      milis=milis-t;
      led_blue_on();
      t=int(b/m);
      delay(t);
      milis=milis-t;      
    }
    led_off();
}

void blink_led_green(int n, int ms) {
  for (int i = 0; i < n; i++) {
    led_green_on();  // turn the LED on (HIGH is the voltage level)
    delay(ms);                   // wait for a second
    led_off();  // turn the LED off by making the voltage LOW
    delay(ms);
  }  
}

void blink_led_red(int n, int ms) {
  for (int i = 0; i < n; i++) {
    led_red_on();  // turn the LED on (HIGH is the voltage level)
    delay(ms);                   // wait for a second
    led_off();  // turn the LED off by making the voltage LOW
    delay(ms);
  }  
}

void led_red_on() {
  digitalWrite(LED_RED,HIGH);
  digitalWrite(LED_BLUE,LOW);
  digitalWrite(LED_GREEN,LOW);
}

void led_green_on() {
  digitalWrite(LED_RED,LOW);
  digitalWrite(LED_BLUE,LOW);
  digitalWrite(LED_GREEN,HIGH);
}

void led_green_set(bool on) {
  digitalWrite(LED_RED,LOW);
  digitalWrite(LED_BLUE,LOW);
  digitalWrite(LED_GREEN,on);
}


void led_blue_on() {
  digitalWrite(LED_RED,LOW);
  digitalWrite(LED_BLUE,HIGH);
  digitalWrite(LED_GREEN,LOW);
}

void led_yellow_on() {
  digitalWrite(LED_RED,HIGH);
  digitalWrite(LED_BLUE,LOW);
  digitalWrite(LED_GREEN,HIGH);
}

void led_yellow_on_t(int ms) {
    led_rgb_tOn(255,50,0,ms); //amarillo
}

void led_off() {
  digitalWrite(LED_RED,LOW);
  digitalWrite(LED_BLUE,LOW);
  digitalWrite(LED_GREEN,LOW);
}

void print_distancias() {
  
  Serial.print("valor real: ");
  Serial.print(analogRead(IN_DIAMETER));
  Serial.print("valor filtrado: ");
  Serial.print(MeasureAnalogN(SAMPLES, IN_DIAMETER));
  Serial.print("diametro calculado: ");
  Serial.println(distancia_rollo(MeasureAnalogN(SAMPLES,IN_DIAMETER)));
}

void print_temporal_tirada() {
  Serial.println("diametro; giros; metros ");
  Serial.print(diametro, 3);
  Serial.print(" ; ");
  Serial.print(vueltas_temp, 2);
  Serial.print(" ; ");
  Serial.println(gasto_temp, 3);
}