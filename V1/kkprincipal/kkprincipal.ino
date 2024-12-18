#include"headers.h"

//código para el reloj
#include "RTClib.h"
#include "LowPower.h"

RTC_DS1307 rtc;

//codigo para lector SD
#include <SPI.h>
#include <SD.h>

const int chipSelect = 10;

const int n_octocoplador = 4;      //número de marcas que detecta el octocoplador para considerar una vuelta.

double vueltas_totales = 0, vueltas_temp = 0;  //contador de vueltas
double sensorDiametro,sensorDiametro2;            //Sensor IR que mide distancia al rollo para estimar su diámetro y cuanto uso lleva
unsigned long cronometro, cronometro_v;
unsigned long dT_line=0; // delta de tiempo en q detecta un cambio de color de línea
int flag_rolling = 0, marca_vuelta = 0;
int halada = 0;
DateTime now;                                                        //variable para marcar fecha y hora
int huella, huella1 ,huellatemp, huella1temp;  //variables para algoritmo de huella digital
int sensorLinea=0; //para almacenar la lectura análoga del sensor de línea
int umbral = 400;   //Umbral de diferenciacion entre negro y blanco.
int radio_min_analog = 30;      // 24mm lectura de los radios análogos
int radio_max_analog = 800;     // 70mm

//const char nombreArchivo = "1kk207.csv";

byte id = 1;

void wakeUp()
{
    // Just a handler for the pin interrupt.
}

void setup() {
  setup_general();
  reloj_setup();
  if(ENABLE_SD) setup_SD();

  int status_setup = espera_configuracion();
  if (status_setup == 0){
    //no se hizo setup, asi que se cargan los valores desde memoria.
    umbral = leerEEPROM(0);
    radio_max_analog=leerEEPROM(2);
    radio_min_analog=leerEEPROM(4);
    Serial.print(umbral);
    Serial.print(";");
    Serial.print(radio_max_analog);
    Serial.print(";");
    Serial.println(radio_min_analog);
  }

  sensorDiametro=VAR_INIT; // Si se lee VAR_INIT en la SD es porq se cortó la electricidad y en este momento volvió
  sensorDiametro2=radio_max_analog;
  vueltas_temp=radio_min_analog;  // 
  now = rtc.now();  //Se guarda el tiempo actual
  if(ENABLE_SD) escritura_SD(); // 

  Serial.println("Proyecto KKs con IR v2.0 Inicializado"); 

  blink_led_green(5,100); 
}

void loop() {   
  //Aqui se debe guardar el estado actual del sensor de linea 
  //-----------------------
  //Se configura el pin para despertar el 
  attachInterrupt(digitalPinToInterrupt(IN_INTERRUPT), wakeUp, CHANGE);

  //Se apaga el arduino
  pinMode(OUT_DISABLE_POWER,INPUT);//Se pone el pin en alta impedancia
  LowPower.powerDown(SLEEP_FOREVER, ADC_OFF, BOD_OFF);

  //Cuando se prenda el arduino aquí se continua
  detachInterrupt(digitalPinToInterrupt(IN_INTERRUPT)); 
  pinMode(OUT_DISABLE_POWER,OUTPUT); //Se pone el pin en escritura
  digitalWrite(OUT_DISABLE_POWER,LOW); //Se pone el pin en low para habilitar VDD

  flag_rolling = 1; //flag para iniciar el cálculo
  now = rtc.now();  //Se guarda el tiempo actual
  calculo();   
}
