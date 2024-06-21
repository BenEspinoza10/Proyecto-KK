#include "headers.h"

void setup_SD() {
  if (!SD.begin(chipSelect)) {

    Serial.println("initialization of SD failed!");
    led_red_on();
    while(true);
    return;
  }

  //escritura inicial de cabecera del archivo csv
  if (!SD.exists("dataKKs.csv")) {
    myFile = SD.open("dataKKs.csv", FILE_WRITE);
    if (myFile) {
      myFile.println("Fecha,Hora,sDiametro,Diametro,Vueltas,Consumo(m),Lecturas");
      Serial.println("Archivo main CSV creado");
    } else {
      Serial.println("Fallo de apertura main CSV");
      led_red_on();
      while(true);
    }
    myFile.close();
  } else {
    Serial.println("Archivo main CSV ya existe");
  }
}

void escritura_SD() {
  //por ahora escribe en una linea de texto, pero se puede formatear a un csv para lectura más ordenada  
  myFile = SD.open("dataKKs.csv", FILE_WRITE);
  if (myFile) {
    myFile.println("");
    myFile.print(now.day(), DEC);
    myFile.print('-');
    myFile.print(now.month(), DEC);
    myFile.print('-');
    myFile.print(now.year(), DEC);
    myFile.print(",");
    myFile.print(now.hour(), DEC);
    myFile.print(':');
    myFile.print(now.minute(), DEC);
    myFile.print(':');
    myFile.print(now.second(), DEC);
    myFile.print(",");    
    myFile.print(sensorDiametro);
    myFile.print(",");    
    myFile.print(diametro);
    myFile.print(",");    
    myFile.print(vueltas_temp, 2);
    myFile.print(",");
    myFile.println(gasto_temp, 1);
    myFile.close();
  } else {
    Serial.println("Error abriendo archivo en la SD main");
    led_red_on();
    while(true);
  }
}

void escritura_SD_temp() {
  myFile = SD.open("dataKKs.csv", FILE_WRITE);
  DateTime nowtemp = rtc.now(); 
  if (myFile) {    
    myFile.print(now.day(), DEC);
    myFile.print('-');
    myFile.print(now.month(), DEC);
    myFile.print('-');
    myFile.print(now.year(), DEC);
    myFile.print(",");
    myFile.print(now.hour(), DEC);
    myFile.print(':');
    myFile.print(now.minute(), DEC);
    myFile.print(':');
    myFile.print(now.second(), DEC);
    myFile.print(","); 
    myFile.print(micros()-ucrono);       
    myFile.print(",");   
    myFile.print(huella);
    myFile.print(",");  
    myFile.close();
  } else {
    Serial.println("Error abriendo archivo en la SD temp");
    led_red_on();
    while(true);
  }

}