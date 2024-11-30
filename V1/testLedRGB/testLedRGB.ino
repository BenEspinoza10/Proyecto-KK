//pines para la luz led
#define LED_RED 5
#define LED_GREEN 6
#define LED_BLUE 7

#define PUSH_BUTTON 9 //pin para el boton

void setup() {
  Serial.begin(115200);
  while (!Serial);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(PUSH_BUTTON, INPUT_PULLUP);
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_BLUE, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);

  //if (digitalRead(PUSH_BUTTON) == 0) {
  //    while (digitalRead(PUSH_BUTTON) == 0)
  //      ;
}

void loop(){
  led_rgb_tOn(255,0,0,1000); // rojo
  delay(500);
  blink_led_green(5,100); // verde 5 veces, 100ms
  led_rgb_tOn(0,0,255,1000); //azul
  delay(500); 
  for (int i=0;i<200;i++)
    led_yellow_on_t(10); //amarillo por 10*200ms
  
  led_rgb_tOn(255,0,210,1000); // morado 
  delay(500); 
  led_rgb_tOn(255,70,100,1000); // blanco
  delay(500);
  //    return 1;
  //  }
  
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

void led_blue_on() {
  digitalWrite(LED_RED,LOW);
  digitalWrite(LED_BLUE,HIGH);
  digitalWrite(LED_GREEN,LOW);
}

void led_yellow_on_t(int ms) {
    led_rgb_tOn(255,50,0,ms); //amarillo
}

void led_off() {
  digitalWrite(LED_RED,LOW);
  digitalWrite(LED_BLUE,LOW);
  digitalWrite(LED_GREEN,LOW);
}


