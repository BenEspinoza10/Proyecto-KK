#include <SPI.h>
#include <SD.h>

File myFile;
const int chipSelect = 10;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  if (!SD.begin(chipSelect)) {

    Serial.println("initialization of SD failed!");
    while (true) {
      while (1);
    }
  }
  myFile = SD.open("dataKKs.csv");
  if(myFile){
    while (myFile.available()) {
      Serial.write(myFile.read());      
    }
  }else {
    // if the file didn't open, print an error:
    Serial.write("error abriendo archivo");
    
  }
}

  void loop() {
    // put your main code here, to run repeatedly:
  }
