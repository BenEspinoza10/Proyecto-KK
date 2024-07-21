#include "headers.h"

void setup_SD() {
  if (!SD.begin(chipSelect)) {
    Serial.println("initialization of SD failed!");
    while (true) {
      blink_led_red(3, 200);
      blink_led_blue(1, 200);
      delay(10000);
    }
    return;
  }
  if (!SD.exists("2kk2107.csv")) {
    File myFile = SD.open("2kk2107.csv", FILE_WRITE);
    if (myFile) {
      myFile.println("2kk2107.csv");      
      myFile.println("FechaHora,sDist,sDist2,Giros,sLinea,DtSeg");
      myFile.close();
      Serial.println("archivo main creado");
    } else {
      Serial.println("Fallo de apertura main");
      while (true) {
        blink_led_red(2, 200);
        blink_led_blue(2, 200);
        delay(10000);
      }
      return;
    }
  }
}

void escritura_SD_temp() {
  File myFile = SD.open("2kk2107.csv", FILE_WRITE);
  //DateTime nowtemp = rtc.now();
  if (myFile) {
    myFile.print(",");
    myFile.print(sensorLinea - umbral, DEC);
    myFile.print(",");
    myFile.print(double(dT_line) / 1000.0, 3);
    myFile.close();
  } else {
    Serial.println("Error abriendo archivo en la SD temp");
    while (true) {
      blink_led_red(4, 200);
      blink_led_blue(1, 200);
      delay(10000);
    }
    return;
  }
}

void escritura_SD() {
  //escritura inicial de cabecera del archivo csv Auxiliar
  File myFile = SD.open("2kk2107.csv", FILE_WRITE);
  if (myFile) {
    myFile.println("");
    myFile.print(now.year(), DEC);
    myFile.print('/');
    myFile.print(now.month(), DEC);
    myFile.print('/');
    myFile.print(now.day(), DEC);
    myFile.print(" ");
    myFile.print(now.hour(), DEC);
    myFile.print(':');
    myFile.print(now.minute(), DEC);
    myFile.print(':');
    myFile.print(now.second(), DEC);
    myFile.print(",");
    myFile.print(sensorDiametro, 1);
    myFile.print(",");
    myFile.print(sensorDiametro2, 1);
    myFile.print(",");
    myFile.print(vueltas_temp, 3);
    if (bool(huella)) {  //si detecta línea negra deja en blanco dos espacios en la tabla para que el CSV quede alineado en columnas para lecturas de línea negra y blanca
      myFile.print(",");
      myFile.print(",");
    }
    myFile.close();
  } else {
    Serial.println("Error abriendo archivo en la SD main");
    while (true) {
      blink_led_red(3, 200);
      blink_led_blue(2, 200);
      delay(10000);
    }
    return;
  }
}