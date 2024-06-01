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

int umbral = 600;   //Umbral de diferenciacion entre negro y blanco. TODO: ahora esta hardcodeado pero la idea es calibrarlo al momento de inicializacion

void wakeUp()
{
    // Just a handler for the pin interrupt.
}

void setup() {
  //pinMode(OUT_DISABLE_POWER, OUTPUT);
  //digitalWrite(OUT_DISABLE_POWER, HIGH); // DESABILITAR CORRIENTE PARA DORMIR SENSORES
  setup_general();
  reloj_setup();
  setup_SD();
  Serial.println("Proyecto KKs con IR v2.0 Inicializado");  
}

void loop() {    
  //Aqui se debe guardar el estado actual del sensor de linea 
  //-----------------------
  //Se configura el pin para despertar el arduino
  attachInterrupt(digitalPinToInterrupt(IN_INTERRUPT), wakeUP, CHANGE);

  //Se apaga el arduino
  pinMode(OUT_DISABLE_POWER,INPUT)//Se pone el pin en alta impedancia
  LowPower.powerDown(SLEEP_FOREVER, ADC_OFF, BOD_OFF);

  //Cuando se prenda el arduino aquí se continua
  detachInterrupt(digitalPinToInterrupt(IN_INTERRUPT)); 
  pinMode(OUT_DISABLE_POWER,OUTPUT); //Se pone el pin en escritura
  digitalWrite(OUT_DISABLE_POWER,LOW); //Se pone el pin en low para habilitar VDD


  flag_rolling = 1; //flag para iniciar el cálculo
  Serial.println("movimiento activado");  
  now = rtc.now();  //Se guarda el tiempo actual
  calculo();  
}