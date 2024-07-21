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

  //escritura inicial de cabecera del archivo csv

  itoa(id, idstr, 10);
  char diatemp[2];
  char mestemp[2];
  itoa(now.day(), diatemp, 10);
  itoa(now.month(), mestemp, 10);
  strcpy(archivoOriginal, idstr);
  strcat(archivoOriginal, "KK");
  strcat(archivoOriginal, diatemp);
  strcat(archivoOriginal, mestemp);
  strcat(archivoOriginal, ".csv");
  Serial.println(archivoOriginal);
  if (!SD.exists(archivoOriginal)) {
    File myFile = SD.open(archivoOriginal, FILE_WRITE);
    if (myFile) {
      myFile.print("id dispositivo: ");
      myFile.println(id);
      myFile.println("Fecha,Hora,sDist,sDist2,sDiam,Giros,DtSeg");
      myFile.close();
      Serial.println("archivo main creado");
    } else {
      Serial.println("Fallo de apertura main");
      while (true) {
        blink_led_red(2, 200);
        blink_led_blue(2, 200);
        delay(10000);
      }
    }
  } else {
    //se detecta que ya existe un archivo
    Serial.println("Archivo main ya existe");
  }
}



void escritura_SD() {
  File myFile = SD.open(archivoOriginal, FILE_WRITE);
  if (myFile) {
    myFile.println("");
    myFile.print(now.year(), DEC);
    myFile.print('/');
    myFile.print(now.month(), DEC);
    myFile.print('/');
    myFile.print(now.day(), DEC);
    myFile.print(",");
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
    myFile.print(diametro, 1);
    myFile.print(",");
    myFile.print(vueltas_temp, 3);
    //myFile.print(",");
    //myFile.print(gasto_temp,4);
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

void escritura_SD_temp() {
  File myFile = SD.open(archivoOriginal, FILE_WRITE);
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

void escritura_SD_main() {
  //escritura inicial de cabecera del archivo csv Auxiliar
  File myFile = SD.open(archivoOriginal, FILE_WRITE);
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
