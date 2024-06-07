#include "headers.h"

//codigo que corre al inicio del codigo, si se aprieta el boton dentro de los primeros 10 segundos mientras la led está amarilla, se entra en modo condiguracion, en caso contrario, se cargan los datos desde la memoria.
int espera_configuracion() {
  Serial.println("esperando configuración");
  led_yellow_on();
  unsigned long tiempoConfig = millis();
  while (millis() - tiempoConfig < 5000) {  //se espera el input del boton por 10 seg
    if (digitalRead(PUSH_BUTTON) == 0) {
      while (digitalRead(PUSH_BUTTON) == 0)
        ;
      configuracion();
      return 1;
    }
  }
  led_off();
  return 0;
}

void configuracion() {
  led_off();
  led_blue_on();
  pinMode(OUT_DISABLE_POWER, OUTPUT);  //Se pone el pin en escritura
  digitalWrite(OUT_DISABLE_POWER, LOW);
  while (digitalRead(PUSH_BUTTON) == 0)
    ;
  Serial.println("Ingrese negro");
  //primero poner el rollo en la línea negra
  while (digitalRead(PUSH_BUTTON) == 1)
    ;
  led_off();

  long sumanegro = 0;
  for (int i = 0; i < 100; i++) {
    led_yellow_on();
        
    sumanegro += analogRead(IN_LINE);
    Serial.println(sumanegro);
    delay(10);
    led_off();
    delay(10);
  }
  int negro = sumanegro / 100;
  led_off();
  led_blue_on();
  while (digitalRead(PUSH_BUTTON) == 0)
    ;


  Serial.println("Ingrese blanco");
  //luego hay que poner el valor del blanco
  while (digitalRead(PUSH_BUTTON) == 1)
    ;
  led_off();

  long sumablanco = 0;
  for (int i = 0; i < 100; i++) {
    led_yellow_on();    
    sumablanco += analogRead(IN_LINE);
    delay(10);
    led_off();
    delay(10);
  }
  int blanco = sumablanco / 100;

  umbral = (blanco + negro) / 2;  //Se calcula el umbral
  while (digitalRead(PUSH_BUTTON) == 0)
    ;
  led_off();
  led_blue_on();
  Serial.println("Ingrese rollo nuevo");
  //luego hay que poner el rollo más grande posible
  while (digitalRead(PUSH_BUTTON) == 1)
    ;
  led_off();

  long sumamax = 0;
  for (int i = 0; i < 100; i++) {
    led_yellow_on();
    sumamax += analogRead(IN_DIAMETER);
    delay(10);
    led_off();
    delay(10);
  }
  radio_max_analog = sumamax / 100;
  while (digitalRead(PUSH_BUTTON) == 0)
    ;
  led_off();
  led_blue_on();


  Serial.println("Ingrese rollo vacío");
  //por ultimo hay que poner un rollo vacío
  while (digitalRead(PUSH_BUTTON) == 1)
    ;
  led_off();

  long sumamin = 0;
  for (int i = 0; i < 100; i++) {
    led_yellow_on();
    sumamin += analogRead(IN_DIAMETER);
    delay(10);
    led_off();
    delay(10);
  }
  radio_min_analog = sumamin / 100;
  led_off();
  led_blue_on();
  while (digitalRead(PUSH_BUTTON) == 0)
    ;

  Serial.println("negro,blanco,umbral; radio min; radiomax ");
  Serial.print(negro);
  Serial.print(" ; ");
  Serial.print(blanco);
  Serial.print(" ; ");
  Serial.print(umbral);
  Serial.print(" ; ");
  Serial.print(radio_max_analog);
  Serial.print(" ; ");
  Serial.println(radio_min_analog);

  escribir_memoria(umbral, radio_max_analog, radio_min_analog);  //se guardan los valores en memoria eeprom
}

void escribir_memoria(int umbral, int radio_max, int radio_min) {
  escribirEEPROM(0, umbral);
  escribirEEPROM(2, radio_max);
  escribirEEPROM(4, radio_min);
}


void escribirEEPROM(uint8_t address, int value) {
  // Descomponer el int en bytes individuales
  uint8_t bytes[2];
  bytes[0] = (value >> 8) & 0xFF;
  bytes[1] = value & 0xFF;

  // Escribir los bytes en la NVRAM
  rtc.writenvram(address, bytes, 2);
}

int leerEEPROM(uint8_t address) {
  // Leer los 2 bytes de la NVRAM
  uint8_t bytes[2];
  rtc.readnvram(bytes, 2, address);

  // Recomponer los bytes en un int
  int value = ((int)bytes[0] << 8) | bytes[1];
  return value;
}