#include "headers.h"

void setup_SD() {
  if (!SD.begin(chipSelect)) {

    Serial.println("initialization of SD failed!");
    while (true){
      blink_led_red(3, 200);
      blink_led_blue(1, 200);
      delay(10000);
    }
    return;
  }

//escritura inicial de cabecera del archivo csv
  if (!SD.exists("dataKKs.csv")) {
    myFile = SD.open("dataKKs.csv", FILE_WRITE);
    if (myFile) {
       myFile.println("Fecha,Hora,sDiametro,Diametro,Vueltas,Consumo,dT_mS,sLinea");
      //myFile.println("Fecha,Hora,%Rollo,Radio,Vueltas,Uso(cms)");
      Serial.println("archivo main creado");
    } else {
      Serial.println("Fallo de apertura main");
      while (true){
        blink_led_red(2, 200);
        blink_led_blue(2, 200);
        delay(10000);
      }
      return;
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
    while (true){
      blink_led_red(3, 200);
      blink_led_blue(2, 200);
      delay(10000);
    }
    return;
  }
}

void escritura_SD_temp() {
  myFile = SD.open("dataKKs.csv", FILE_WRITE);
  DateTime nowtemp = rtc.now(); 
  if (myFile) {    
    myFile.print(","); 
    myFile.print((millis()-cronometro),DEC);       
    myFile.print(",");   
    myFile.print(sensorLinea,4);
    myFile.close();
  } else {
    Serial.println("Error abriendo archivo en la SD temp");
    while (true){
      blink_led_red(4, 200);
      blink_led_blue(1, 200);
      delay(10000);
    }
    return;
  }

}