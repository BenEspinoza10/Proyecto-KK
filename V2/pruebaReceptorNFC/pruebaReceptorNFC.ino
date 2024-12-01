#include <Wire.h>
#include <SPI.h>
#include <Adafruit_PN532.h>
#include <SD.h>

// Configuración del PN532 en I2C
#define SDA_PIN A4
#define SCL_PIN A5
Adafruit_PN532 nfc(SDA_PIN, SCL_PIN);

// Configuración del SD
#define SD_CS_PIN 4

void setup() {
  Serial.begin(9600);
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
  File myFile = SD.open("recibido.csv", FILE_WRITE);
  if (!myFile) {
    Serial.println("Error al abrir archivo para escritura.");
    return;
  }

  uint8_t buffer[16];  // Tamaño de un bloque de datos
  bool receiving = false;

  while (true) {
    if (nfc.ntag2xx_ReadPage(4, buffer)) {
      String packet = "";
      for (int i = 0; i < 4; i++) {
        packet += (char)buffer[i];
      }

      // Detectar inicio de transmisión
      if (packet.startsWith("START")) {
        receiving = true;
        Serial.println("Inicio de transmisión detectado.");
        continue;
      }

      // Detectar fin de transmisión
      if (packet.startsWith("END")) {
        Serial.println("Fin de transmisión detectado.");
        break;
      }

      // Guardar datos si estamos recibiendo
      if (receiving) {
        myFile.print(packet);  // Guardar datos en el archivo
        Serial.print(packet);  // Mostrar datos en el monitor serial
      }

      // Enviar ACK al emisor
      nfc.ntag2xx_WritePage(5, (uint8_t *)"ACK ");
    } else {
      // Enviar NACK si la lectura falló
      nfc.ntag2xx_WritePage(5, (uint8_t *)"NACK");
    }
  }

  myFile.close();  // Cerrar archivo
  Serial.println("Archivo guardado correctamente.");
  delay(5000);
}
