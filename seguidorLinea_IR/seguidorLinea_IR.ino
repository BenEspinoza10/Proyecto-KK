#include"headers.h"
#include <EEPROM.h>
//código para el reloj
#include "RTClib.h"
#include "LowPower.h"

RTC_DS1307 rtc;

//codigo para lector SD
#include <SPI.h>
#include <SD.h>

File myFile;
const int chipSelect = 10;

int n_octocoplador = 4;      //número de marcas que detecta el octocoplador para considerar una vuelta.
double radio_min = 2.5;      //radio del cartón en cm
double radio_max = 6.5;      //radio de un rollo nuevo en cm
int timeout_halada = 5.000;  //tiempo en segundos que se considera de inactividad mínima entre jalón de papel

double vueltas_totales = 0, vueltas_temp = 0;  //contador de vueltas
double sensorRadio = 0, radio_temp;            //Sensor IR que mide distancia al rollo para estimar cuanto uso lleva
double gasto_temp, gasto_total;                //Contador de gasto, en cm
unsigned long cronometro;
int flag_rolling = 0, marca_vuelta = 0;
int halada = 0;
DateTime now;                                                        //variable para marcar fecha y hora
int huella, huella1 ,huellatemp, huella1temp;  //variables para algoritmo de huella digital
int ID = 0;


void setup() {
  //pinMode(OUT_DISABLE_POWER, OUTPUT);
  //digitalWrite(OUT_DISABLE_POWER, HIGH); // DESABILITAR CORRIENTE PARA DORMIR SENSORES
  setup_general();
  reloj_setup();
  setup_SD();
  Serial.println("Proyecto KKs con IR v2.0 Inicializado");
  blink_led_builin(200, 3);  //prender led 2 veces por 200ms
}

void loop() {    
  int status = digitalRead(IN_LINE);

  while(status == digitalRead(IN_LINE)){
    continue;
  }
  flag_rolling = 1;
  Serial.println("movimiento activado");
  //se pone el arduino en sleep
  //LowPower.powerDown(SLEEP_FOREVER, ADC_OFF, BOD_OFF);

  //se despierta el arduino cuando detecta marca en el optocoplador
  
  now = rtc.now();  
  calculo();  
}