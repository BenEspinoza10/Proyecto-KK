void setup_SD() {
  if (!SD.begin(chipSelect)) {
    Serial.println("initialization of SD failed!");
    return;
  }

  //escritura inicial de cabecera del archivo csv
  if (!SD.exists("dataKKs.csv")) {
    myFile = SD.open("dataKKs.csv", FILE_WRITE);
    if (myFile) {
      myFile.println("Fecha,Hora,%Rollo,Radio,Vueltas,Uso(cms)");
      Serial.println("archivo main creado");
    } else {
      Serial.println("Fallo de apertura main");
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
    Serial.println("Error abriendo archivo en la SD main");
  }
}

void escritura_SD_temp() {
  myFile = SD.open("dataKKs.csv", FILE_WRITE);
  DateTime nowtemp = rtc.now(); 
  if (myFile) {
    myFile.print(nowtemp.day(), DEC);
    myFile.print('/');
    myFile.print(nowtemp.month(), DEC);
    myFile.print('/');
    myFile.print(nowtemp.year(), DEC);
    myFile.print(",");
    myFile.print(nowtemp.hour(), DEC);
    myFile.print(':');
    myFile.print(nowtemp.minute(), DEC);
    myFile.print(':');
    myFile.print(nowtemp.second(), DEC);
    myFile.print(","); 
    myFile.print(cronometro);       
    myFile.print(",");   
    myFile.print(huella);
    myFile.print(",|||,");  
    myFile.close();
  } else {
    Serial.println("Error abriendo archivo en la SD temp");
  }
}