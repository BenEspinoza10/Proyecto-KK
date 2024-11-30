#include <SdFat.h>
#include <sdios.h>

#define SD_FAT_TYPE 3
#define SPI_CLOCK SD_SCK_MHZ(50)

const int SD_CS_PIN = 10;

SdFs SD;

#if HAS_SDIO_CLASS
#define SD_CONFIG SdioConfig(FIFO_SDIO)
#elif ENABLE_DEDICATED_SPI
#define SD_CONFIG SdSpiConfig(SD_CS_PIN, DEDICATED_SPI, SPI_CLOCK)
#else  // HAS_SDIO_CLASS
#define SD_CONFIG SdSpiConfig(SD_CS_PIN, SHARED_SPI, SPI_CLOCK)
#endif  // HAS_SDIO_CLASS

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("Empezó el código");
  if (!SD.begin(SD_CONFIG)) {
    Serial.println("initialization of SD failed!");
    while (true)
      ;
  }
  for (int id = 1; id < 73; id++) {
    char archivoOriginal[20];
    char idstr[2];
    strcpy(archivoOriginal, "dataKKs");    
    itoa(id,idstr,10);
    strcat(archivoOriginal, idstr);    
    strcat(archivoOriginal, ".csv");
    //Serial.print("probando archivo ");
    //Serial.println(archivoOriginal);
    if (SD.exists(archivoOriginal)) {      
      FsFile myFile;
      int temp = 1;
      char tempstr[2];
      itoa(temp,tempstr,10);
      char newFileName[25];
      //se renombra con un numero unico, en caso de haber varios archivos antiguos, ni uno se pierda
      do {
        itoa(temp,tempstr,10);
        strcpy(newFileName, "dataKKs");
        strcat(newFileName, idstr);
        strcat(newFileName, "old");
        strcat(newFileName, tempstr);
        strcat(newFileName, ".csv");
        temp++;
      } while (SD.exists(newFileName));
      //se abre el archivo encontrado
      if (!myFile.open(archivoOriginal, O_WRONLY | O_APPEND)) {
        Serial.print("Fallo de apertura de archivo:");
        Serial.println(archivoOriginal);
        while (true)
          ;
      }
      //Se renombra el archivo encontrado
      if (!myFile.rename(newFileName)) {
        Serial.println("Error al renombrar el archivo");
        while (true)
          ;
      }
      Serial.print("Se renombró el archivo ");
      Serial.print(archivoOriginal);
      Serial.print(" por ");
      Serial.println(newFileName);
      myFile.close();
    }
  }
  Serial.println("terminó el código");
}

void loop() {
  // put your main code here, to run repeatedly:
}
