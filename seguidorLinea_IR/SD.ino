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
      myFile.println("Fecha,Hora,sDiametro,Diametro,Vueltas,Consumo(m),DuSeg,UmbralN");
      //myFile.println("Fecha,Hora,%Rollo,Radio,Vueltas,Uso(cms)");
      Serial.println("archivo main creado");
    } else {
      Serial.println("Fallo de apertura main");
      led_red_on();
      while(true);
    }
    myFile.close();
  } else {
    Serial.println("Archivo main ya existe");
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
    myFile.print(sensorDiametro,1);
    myFile.print(",");    
    myFile.print(diametro,4);
    myFile.print(",");    
    myFile.print(vueltas_temp,3);
    myFile.print(",");
    myFile.print(gasto_temp,4);
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
    myFile.print(","); 
    myFile.print((millis()-cronometro),DEC);       
    myFile.print(",");   
    //myFile.print(huella);
    myFile.print(sensorLinea,DEC);
    myFile.close();
  } else {
    Serial.println("Error abriendo archivo en la SD temp");
    led_red_on();
    while(true);
  }

}