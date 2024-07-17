#include "headers.h"

void setup_SD(int status_setup) {
  if (!SD.begin(SD_CONFIG)) {
    Serial.println("initialization of SD failed!");
    while (true) {
      blink_led_red(3, 200);
      blink_led_blue(1, 200);
      delay(10000);
    }
    return;
  }
  //escritura inicial de cabecera del archivo csv  
  char archivoOriginal[20];
  strcpy(archivoOriginal,"dataKKs");
  strcat(archivoOriginal,char(id));
  strcat(archivoOriginal,".csv");    
  if (!SD.exists(archivoOriginal)) {
    FsFile myFile;
    if (!myFile.open(archivoOriginal, O_WRONLY | O_APPEND)) {
      Serial.println("Fallo de apertura main");
      while (true) {
        blink_led_red(2, 200);
        blink_led_blue(2, 200);
        delay(10000);
      }
      return;
    }
    myFile.print("id dispositivo: ");
    myFile.println(id);
    myFile.println("Fecha,Hora,sDist,sDist2,sDiam,Giros,DtSeg");
    myFile.close();
    Serial.println("archivo main creado");
  } else {
    //se detecta que ya existe un archivo
    FsFile myFile;
    Serial.println("Archivo main ya existe");
    //solo hace el renombramiento de archivo si es que se hizo el setup
    if (status_setup) {
      int i = 1;
      char newFileName[25];
      //se renombra con un numero unico, en caso de haber varios archivos antiguos, ni uno se pierda
      do {
        strcpy(newFileName,"dataKKs");
        strcat(newFileName,char(id));
        strcat(newFileName,"old");  
        strcat(newFileName,char(i));
        strcat(newFileName,"csv");            
        i++;
      } while (SD.exists(newFileName));            

      //se hace el renombramiento
      if (!myFile.open(archivoOriginal, O_WRONLY | O_APPEND)) {
        Serial.println("Fallo de apertura main");
        while (true) {
          blink_led_red(2, 200);
          blink_led_blue(2, 200);
          delay(10000);
        }
        return;
      }
      if (!myFile.rename(newFileName)) {
        Serial.println("Fallo de apertura main");
        while (true) {
          blink_led_red(2, 200);
          blink_led_blue(2, 200);
          delay(10000);
        }
        return;
      }
      myFile.close();
      //se crea un nuevo archivo con el id del dispositivo
      if (!myFile.open(archivoOriginal, O_WRONLY | O_APPEND)) {
        Serial.println("Fallo de apertura main");
        while (true) {
          blink_led_red(2, 200);
          blink_led_blue(2, 200);
          delay(10000);
        }
        return;
      }
      myFile.print("id dispositivo: ");
      myFile.println(id);
      myFile.println("Fecha,Hora,sDist,sDist2,sDiam,Giros,DtSeg");
      myFile.close();
      Serial.println("archivo main creado");
    }
  }
}

void escritura_SD() {
  FsFile myFile;
  char archivoOriginal[20];
  strcpy(archivoOriginal,"dataKKs");
  strcat(archivoOriginal,char(id));
  strcat(archivoOriginal,".csv");
  if (!myFile.open(archivoOriginal, O_WRONLY | O_APPEND)) {
    Serial.println("Error abriendo archivo en la SD main");
    while (true) {
      blink_led_red(3, 200);
      blink_led_blue(2, 200);
      delay(10000);
    }
    return;
  }
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
  myFile.close();
}

void escritura_SD_temp() {
  FsFile myFile;
  char archivoOriginal[20];
  strcpy(archivoOriginal,"dataKKs");
  strcat(archivoOriginal,char(id));
  strcat(archivoOriginal,".csv");
  if (!myFile.open(archivoOriginal, O_WRONLY | O_APPEND)) {
    Serial.println("Error abriendo archivo en la SD temp");
    while (true) {
      blink_led_red(4, 200);
      blink_led_blue(1, 200);
      delay(10000);
    }
    return;
  }
  myFile.print(",");
  myFile.print(double(millis() - cronometro) / 1000.0, 3);
  myFile.close();
}
