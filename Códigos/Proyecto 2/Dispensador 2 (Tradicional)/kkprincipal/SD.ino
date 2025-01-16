#include "headers.h"
//TODO: Hay que revisar la escritura main y la temporal
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
  if (!SD.exists("50kk2607.csv")) {
    File myFile = SD.open("50kk2607.csv", FILE_WRITE);
    if (myFile) {
      myFile.println("50kk2607.csv");
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
  File myFile = SD.open("50kk2607.csv", FILE_WRITE);
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
  File myFile = SD.open("50kk2607.csv", FILE_WRITE);
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

void escritura_SD_apertura() {
  File myFile = SD.open("50kk2607.csv", FILE_WRITE);
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
    myFile.println("¡¡¡¡¡¡¡Se hizo un cambio de rollo!!!!!!!!");
    myFile.close();
  } else {
    Serial.println("Error abriendo archivo en la SD apertura");
    while (true) {
      blink_led_red(3, 200);
      blink_led_blue(2, 200);
      delay(10000);
    }
    return;
  }
}

void escritura_SD_transferencia() {
  File myFile = SD.open("50kk2607.csv", FILE_WRITE);
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
    myFile.println("¡¡¡¡¡¡¡Se hizo transferencia de archivo!!!!!!!!");
    myFile.close();
  } else {
    Serial.println("Error abriendo archivo en la SD transferencia");
    while (true) {
      blink_led_red(3, 200);
      blink_led_blue(2, 200);
      delay(10000);
    }
    return;
  }
}

bool transferir_archivos() {
  //Se pone el arduino en modo de espera para el bluetooth
  if (bluetooth.available() > 0) {
    //Al momento de recibir la señal de la app, se inicia la transmicion de datos
    File myFile = SD.open("50kk2607.csv");
    if (myFile) {
      Serial.println(F("Enviando datos al Bluetooth..."));
      // Leer y enviar el contenido del archivo por Bluetooth
      while (myFile.available()) {
        char data = myFile.read();
        bluetooth.write(data);  // Enviar datos al bluetooth
        Serial.write(data);     // También mostrar en el monitor serial para debug
      }

      myFile.close();  // Cerrar el archivo
      bluetooth.println("\nTransferencia completada.");
      Serial.println(F("\nTransferencia completada."));
    } else {
      Serial.println(F("No se pudo abrir el archivo CSV."));
      bluetooth.println("Error: No se pudo abrir el archivo CSV.");
    }
    while (bluetooth.read() >= 0)
      ;
    return true;
  }
  return false;
}