#include "headers.h"

void setup_SD(int status_setup) {
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
  if (!SD.exists("dataKKs.csv")) {
    File myFile = SD.open("dataKKs.csv", FILE_WRITE);
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
      return;
    }
  } else {
    //se detecta que ya existe un archivo
    Serial.println("Archivo main ya existe");
    //solo hace el renombramiento de archivo si es que se hizo el setup
    if (status_setup) {
      int i = 1;
      String newFileName;
      //se renombra con un numero unico, en caso de haber varios archivos antiguos, ni uno se pierda
      do {
        newFileName = "dataKKsold" + String(i) + ".csv";
        i++;
      } while (SD.exists(newFileName));

      //se hace el renombramiento
      if (SD.rename("dataKKs.csv", newFileName.c_str())) {
        Serial.println("Archivo renombrado a " + newFileName);
      } else {
        Serial.println("Fallo al renombrar el archivo");
        while (true) {
          blink_led_red(2, 200);
          blink_led_blue(2, 200);
          delay(10000);
        }
        return;
      }

      //se crea un nuevo archivo con el id del dispositivo
      File myFile = SD.open("dataKKs.csv", FILE_WRITE);
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
        return;
      }
    }
  }
}

void escritura_SD() {
  File myFile = SD.open("dataKKs.csv", FILE_WRITE);
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
  File myFile = SD.open("dataKKs.csv", FILE_WRITE);
  //DateTime nowtemp = rtc.now();
  if (myFile) {
    myFile.print(",");
    myFile.print(double(millis() - cronometro) / 1000.0, 3);
    //myFile.print(",");
    //myFile.print(sensorLinea,DEC);
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
