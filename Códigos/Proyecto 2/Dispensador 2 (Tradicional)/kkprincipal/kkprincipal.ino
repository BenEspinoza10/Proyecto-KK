#include "headers.h"

//código para el reloj
#include "RTClib.h"
#include "LowPower.h"
RTC_DS1307 rtc;

//codigo para lector SD
#include <SPI.h>
#include <SD.h>
const int chipSelect = 10;

//codigo para el bluetooth
#include <SoftwareSerial.h>
SoftwareSerial bluetooth(BT_RX, BT_TX);  // Crear objeto SoftwareSerial para Bluetooth

bool transferenciaBT = false;  //Flag para indicar que el dispositivo está abierto

//codigo para el láser
#include <Wire.h>
#include <VL53L0X.h>

VL53L0X laser;


//TODO: Esto es anitguo, revisar que hay que cambiar y que hay que mantener------------------------------------------------------------------//
const int n_octocoplador = 4;                  //número de marcas que detecta el octocoplador para considerar una vuelta.                    //
double vueltas_totales = 0, vueltas_temp = 0;  //contador de vueltas                                                                         //
double sensorDiametro, sensorDiametro2;        //Sensor IR que mide distancia al rollo para estimar su diámetro y cuanto uso lleva           //
unsigned long cronometro, cronometro_v;        //                                                                                            //
unsigned long dT_line = 0;                     // delta de tiempo en q detecta un cambio de color de línea                                   //
int flag_rolling = 0, marca_vuelta = 0;        //                                                                                            //
int halada = 0;                                //                                                                                            //
DateTime now;                                  //variable para marcar fecha y hora                                                           //
int huella, huella1, huellatemp, huella1temp;  //variables para algoritmo de huella digital                                                  //
int sensorLinea = 0;                           //para almacenar la lectura análoga del sensor de línea                                       //
int umbral = 400;                              //Umbral de diferenciacion entre negro y blanco.                                              //
int radio_min_analog = 30;                     // 24mm lectura de los radios análogos                                                        //
int radio_max_analog = 800;                    // 70mm                                                                                       //
//const char nombreArchivo = "1kk207.csv";                                                                                                   //
byte id = 1;  //                                                                                                                             //
//-------------------------------------------------------------------------------------------------------------------------------------------//


//Funciones para manejar las interrupciones
void wakeUpVibracion() {
  // Just a handler for the pin interrupt.
}

void wakeUpBT() {
  transferenciaBT = true;
  Serial.print(F("Se abrió el dispositivo"));
}

void setup() {
  setup_general();
  reloj_setup();
  laser_setup();
  if (ENABLE_SD) setup_SD();

  int status_setup = espera_configuracion();
  if (status_setup == 0){
    //no se hizo setup, asi que se cargan los valores desde memoria.
    umbral = leerEEPROM(0);    
    Serial.print(F("Umbral Guardado"));
    Serial.println(umbral);    
  }

  now = rtc.now();                //Se guarda el tiempo actual
  if (ENABLE_SD) escritura_SD();  

  Serial.println(F("Proyecto 2 KKs con INFRARROJO v1.0 Inicializado"));

  blink_led_green(5, 100);
}

void loop() {

  //Se configura el pin para despertar el ARDUINO
  attachInterrupt(digitalPinToInterrupt(IN_VIBRACION), wakeUpVibracion, CHANGE);
  attachInterrupt(digitalPinToInterrupt(IN_BUTTON), wakeUpBT, CHANGE);

  //Se apaga el arduino
  pinMode(OUT_DISABLE_POWER_IR, INPUT);  //Se pone el pin en alta impedancia
  pinMode(OUT_DISABLE_POWER_BT, INPUT);  //Se pone el pin en alta impedancia
  LowPower.powerDown(SLEEP_FOREVER, ADC_OFF, BOD_OFF);

  //Cuando se prenda el arduino aquí se continua
  detachInterrupt(digitalPinToInterrupt(IN_VIBRACION));
  detachInterrupt(digitalPinToInterrupt(IN_BUTTON));

  //Se verifica si se aprieta el botón o no
  if (transferenciaBT) {
    pinMode(OUT_DISABLE_POWER_BT, OUTPUT);  //Se pone el pin en escritura para darle corriente al bluetooth
    digitalWrite(OUT_DISABLE_POWER_BT, HIGH);
    escritura_SD_apertura();
    bluetooth.begin(9600);  // Comunicación Serial con el bluetooth
    Serial.println(F("Inicializando Bluetooth"));
    unsigned long tiempoBT = millis();
    while (millis() - tiempoBT < T_CONFIG) {  //Aqui se entra en un loop mientras el dispositivo está abierto TODO:revisar el estado de la lectura cuando está abierto el dispositivo
      if (transferir_archivos()){
        escritura_SD_transferencia();  //Se escribe un header en el archivo para saber el punto de lectura de archivos
        break;        
      }
    }    
    transferenciaBT = false;            //Se vuelve al estado inicial para volver a apagar el arduino
  }
  //caso contrario, se hace el calculo de ida al baño normalmente
  else {
    flag_rolling = 1;  //flag para iniciar el cálculo
    now = rtc.now();   //Se guarda el tiempo actual
    pinMode(OUT_DISABLE_POWER_IR, OUTPUT);  //Se pone el pin en escritura para darle corriente al bluetooth
    digitalWrite(OUT_DISABLE_POWER_BT, HIGH);
    calculo();
  }
}
