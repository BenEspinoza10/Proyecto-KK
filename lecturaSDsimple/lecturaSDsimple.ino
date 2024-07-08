//Fecha,Hora,sDist,sDist2,sDiam,Giros,DtSeg,sLinea



#include <SPI.h>
#include <SD.h>

File myFile;
const int chipSelect = 10;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  if (!SD.begin(chipSelect)) {

    Serial.println("initialization of SD failed!");
    while (true) {
      while (1);
    }
  }
  myFile = SD.open("dataKKs.csv");
  if(!myFile){
          // if the file didn't open, print an error:
    Serial.write("error abriendo archivo");
    myFile.close();
    while (true) {while (1);}
  } else{
    while (myFile.available()) {
    Serial.write(myFile.read());      
    }
    myFile.close();
    
   /* myFile = SD.open("dataKKs.csv", FILE_WRITE);
    if (myFile) {
      myFile.println("zzz,Hora,%Rollo,Radio,Vueltas,Uso(cms)");
      myFile.close();
    }*/
  }
}

  void loop() {
    // put your main code here, to run repeatedly:
  }
