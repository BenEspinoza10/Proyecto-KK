#include <Wire.h>
#include <SPI.h>
#include <Adafruit_PN532.h>
#include <SD.h>

// Configuración del PN532 en I2C
#define SDA_PIN A4
#define SCL_PIN A5
Adafruit_PN532 nfc(SDA_PIN, SCL_PIN);

// Configuración del SD
#define SD_CS_PIN 10

void setup() {
  Serial.begin(115200);
  Serial.println("Inicializando receptor...");

  // Inicializar SD
  if (!SD.begin(SD_CS_PIN)) {
    Serial.println("Error al inicializar la tarjeta SD.");
    while (1);
  }
  Serial.println("Tarjeta SD inicializada.");

  // Inicializar NFC
  nfc.begin();
  uint32_t versiondata = nfc.getFirmwareVersion();
  if (!versiondata) {
    Serial.println("No se encontró PN532.");
    while (1);
  }
  nfc.SAMConfig();
  Serial.println("PN532 inicializado en modo receptor.");
}

void loop() {
  // Crear o abrir archivo para escritura
  File myFile = SD.open("recibido.csv", FILE_WRITE);
  if (!myFile) {
    Serial.println("Error al abrir archivo para escritura.");
    return;
  }

  uint8_t buffer[16];  // Tamaño de un bloque de datos
  if (nfc.ntag2xx_ReadPage(4, buffer)) {
    for (int i = 0; i < 4; i++) {
      myFile.write(buffer[i]);  // Guardar datos en SD
      Serial.write(buffer[i]); // Mostrar datos recibidos
    }
    Serial.println("\nDatos guardados.");
  } else {
    Serial.println("Error al leer datos.");
  }

  myFile.close();  // Cerrar archivo
  delay(1000);     // Esperar antes de intentar leer de nuevo
}
