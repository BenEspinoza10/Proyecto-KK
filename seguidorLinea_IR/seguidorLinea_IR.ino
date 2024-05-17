//codigo para poner en modo sleep
#include "LowPower.h"
#include <EEPROM.h>
#define IN_LINE 2
#define OUT_DISABLE_POWER PIN_A6
#define IN_DIAMETER A7  //OJO ANTES ERA A0
//#define OUT_LED PIN_A6 // OJO HAY Q CONFIGURAR A6 COMO SALIDA DIGITAL PARA USAR EL LED DEL SENSOR, prende con cero

#define SAMPLES 10  //No. Muestras del sensor análogo
#define T_FILTER 2  //mSeg sensando el sensor de línea

//código para el reloj
#include "RTClib.h"

RTC_DS1307 rtc;

//codigo para lector SD
#include <SPI.h>
#include <SD.h>

File myFile;
const int chipSelect = 10;

int n_octocoplador = 4;      //número de marcas que detecta el octocoplador para considerar una vuelta.
double radio_min = 2.5;      //radio del cartón en cm
double radio_max = 6.5;      //radio de un rollo nuevo en cm
int timeout_halada = 2.000;  //tiempo en segundos que se considera de inactividad mínima entre jalón de papel

double vueltas_totales = 0, vueltas_temp = 0;  //contador de vueltas
double sensorRadio = 0, radio_temp;            //Sensor IR que mide distancia al rollo para estimar cuanto uso lleva
double gasto_temp, gasto_total;                //Contador de gasto, en cm
unsigned long cronometro;
int flag_rolling = 0, marca_vuelta = 0;
int halada = 0;
DateTime now;                                                        //variable para marcar fecha y hora
int huella, huellatemp, huella1temp;  //variables para algoritmo de huella digital

void wakeUp() {
  // Just a handler for the pin interrupt.
  huella1 = 1;
}

void setup() {
  // put your setup code here, to run once:

  Serial.begin(115200);
  while (!Serial)
    ;

  Wire.begin();
  pinMode(IN_LINE, INPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  //pinMode(OUT_DISABLE_POWER, OUTPUT);
  //digitalWrite(OUT_DISABLE_POWER, HIGH); // DESABILITAR CORRIENTE PARA DORMIR SENSORES

  if (!SD.begin()) {
    Serial.println("initialization of SD failed!");
    return;
  }

  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    //while (1) delay(10);
    return;
  }

  //escritura inicial de cabecera del archivo csv
  if (!SD.exists("dataKKs.csv")) {
    myFile = SD.open("dataKKs.csv", FILE_WRITE);
    myFile.println("Fecha,Hora,%Rollo,Radio,Vueltas,Uso(cms)");
    myFile.close();
  }

  const unsigned int buildTimeEEPROMaddress = 1000;
  const char storedBuildTime[9];
  EEPROM.get(buildTimeEEPROMaddress, storedBuildTime);
  if (strcmp(__TIME__, storedBuildTime) == 0) {
    Serial.println("This is not the first run after upload");
  } else {
    Serial.println("This is the first run after upload");
    EEPROM.put(buildTimeEEPROMaddress, __TIME__);
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    Serial.println("Real Time Clock initialized");
  }

  //Código para setear la hora del reloj sólo si no estaba seteada de antes.
  if (!rtc.isrunning()) {
    Serial.println("RTC is NOT running, let's set the time!");
    // When time needs to be set on a new device, or after a power loss, the
    // following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    Serial.println("Real Time Clock initialized");
  }
  Serial.println("Proyecto KKs con IR v1.2 Inicializado");
  blink_led_builin(200, 3);  //prender led 2 veces por 200ms
}

void loop() {
  //se inicializa el interrupt para prender el arduino de sleep
  //se define el interrup en base a la posición final del octocoplador
  int status = digitalRead(IN_LINE);
  if (status == 0) {
    attachInterrupt(0, wakeUp, HIGH);
  } else {
    attachInterrupt(0, wakeUp, LOW);
  }

  //se pone el arduino en sleep
  LowPower.powerDown(SLEEP_FOREVER, ADC_OFF, BOD_OFF);

  //se despierta el arduino cuando detecta marca en el optocoplador
  detachInterrupt(0);

  //pinMode(OUT_DISABLE_POWER, OUTPUT);
  //digitalWrite(OUT_DISABLE_POWER, LOW); // HABILITAR CORRIENTE
  //iniciar módulos RTC y SD

  flag_rolling = 1;
  now = rtc.now();
  //Serial.println("Arduino despierto, iniciando cálculo");
  calculo();
}

void calculo() {

  cronometro = millis();
  while (flag_rolling == 1) {
    //Leer y filtrar puerto del IR, si es ruido salir
    //huella = MeasureDigitalN(SAMPLES, IN_LINE);
    huella = MeasureDigitalT(T_FILTER, IN_LINE);  //Sensa durante 1.0 mS para filtrar ruido
    if (huella < 0) return;

    digitalWrite(LED_BUILTIN, bool(huella));
    huellatemp = huella;

    if (huella1temp != huellatemp) {
      Serial.print("Sensor: ");
      Serial.println(bool(huella));      
      vueltas_temp++;
      cronometro = millis();            
      huella1temp = huellatemp;
    }

    if (millis() - cronometro > timeout_halada * 1000) {
      //fin evento halada de papel, se hacen los cálculos respectivos, actualmente está definido con 2 segundos
      //digitalWrite(LED_BUILTIN, LOW);
      vueltas_temp = vueltas_temp / n_octocoplador * 0.5;
      vueltas_totales += vueltas_temp;
      halada++;

      //aqui hay que poner el cálculo de los cm usados
      // Leer y normalizar sensor IR de diametro: Aprox 0.7 rollo vacío - 0.02 rollo nuevo
      radio_temp = (1 - MeasureAnalogN(SAMPLES, IN_DIAMETER));
      sensorRadio = radio_max * radio_temp;
      //gasto_temp = 2*M_PI*radio_min*vueltas_temp;
      gasto_temp = 2 * M_PI * sensorRadio * vueltas_temp;


      //escritura de datos en la tarjeta SD
      escritura_SD();

      Serial.println("%rollo; radio; giro ;  cms ");
      Serial.print(radio_temp * 100, 0);
      Serial.print(" ; ");
      Serial.print(sensorRadio, 2);
      Serial.print(" ; ");
      Serial.print(vueltas_temp, 2);
      Serial.print(" ; ");
      Serial.println(gasto_temp, 1);

      vueltas_temp = 0;
      gasto_temp = 0;
      flag_rolling = 0;
    }
  }
}

void escritura_SD() {
  //por ahora escribe en una linea de texto, pero se puede formatear a un csv para lectura más ordenada
  myFile = SD.open("dataKKs.csv", FILE_WRITE);
  if (myFile) {
    //myFile.print(halada);
    //myFile.print(",");

    //myFile.println("Fecha,Hora,Sensor_dist,Vueltas,Diametro(cm),Uso(cms)");
    myFile.print(now.day(), DEC);
    myFile.print('/');
    myFile.print(now.month(), DEC);
    myFile.print('/');
    myFile.print(now.year(), DEC);
    myFile.print(",");
    myFile.print(now.hour(), DEC);
    myFile.print(':');
    myFile.print(now.minute(), DEC);
    myFile.print(':');
    myFile.print(now.second(), DEC);
    myFile.print(",");
    myFile.print(radio_temp * 100, 0);
    myFile.print(",");
    myFile.print(sensorRadio, 2);
    myFile.print(",");
    myFile.print(vueltas_temp, 2);
    myFile.print(",");
    myFile.println(gasto_temp, 1);
    myFile.close();
  } else {
    Serial.println("Error abriendo archivo en la SD");
  }
}

void blink_led_builin(int time_hi, int hm_times) {
  for (int i = 0; i < hm_times; i++) {
    digitalWrite(LED_BUILTIN, HIGH);  // turn the LED on (HIGH is the voltage level)
    delay(time_hi);                   // wait for a second
    digitalWrite(LED_BUILTIN, LOW);   // turn the LED off by making the voltage LOW
    delay(time_hi);
  }  // wait for a second
}

int MeasureDigitalN(unsigned int samplesNumber, int digitalPort) {
  int cum = digitalRead(digitalPort);

  for (int i = 0; i < samplesNumber; i++) {
    if (cum != digitalRead(digitalPort)) return -1;
  }
  return cum;
}


int MeasureDigitalT(unsigned int sensingTime, int digitalPort) {
  unsigned long lapse = millis();
  int cum = digitalRead(digitalPort);
  while (millis() - lapse < sensingTime) {
    if (cum != digitalRead(digitalPort)) return -1;
  }
  return cum;
}

float MeasureAnalogN(unsigned int samplesNumber, int analogPort) {
  //digitalWrite(OUT_DISABLE_POWER, LOW); // HABILITAR CORRIENTE PARA SENSORES
  //delay(10); // Delay para esperar que el sensor de distancia esté activo
  float sum = 0;
  for (int i = 0; i < samplesNumber; i++) {
    sum += analogRead(analogPort) / 1023.0;
  }
  //digitalWrite(OUT_DISABLE_POWER, HIGH); // DESABILITAR CORRIENTE PARA SENSORES
  return (sum / samplesNumber);
}
