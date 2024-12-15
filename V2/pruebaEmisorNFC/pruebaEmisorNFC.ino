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
  Serial.println("Inicializando emisor...");

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
  Serial.println("PN532 inicializado.");
}

void loop() {
  Serial.println("Esperando receptor NFC (celular)...");

  // Intentar detectar un dispositivo NFC cercano
  uint8_t success = nfc.inListPassiveTarget();
  if (success) {
    Serial.println("Celular NFC detectado. Iniciando transmisión...");

    // Abrir el archivo CSV
    File myFile = SD.open("a.csv");
    if (!myFile) {
      Serial.println("Error al abrir el archivo CSV.");
      delay(5000);
      return;
    }

    // Leer y enviar datos en bloques
    while (myFile.available()) {
      char buffer[16];
      memset(buffer, 0, sizeof(buffer)); // Limpiar el buffer
      for (int i = 0; i < 16 && myFile.available(); i++) {
        buffer[i] = myFile.read();
      }

      // Enviar paquete de datos
      enviarPaquete(buffer);
      delay(50); // Pequeña pausa para evitar saturación
    }

    Serial.println("Transmisión completada.");
    myFile.close();
  }

  delay(1000);  // Esperar antes de volver a intentar detectar
}

void enviarPaquete(const char *data) {
  uint8_t success = nfc.ntag2xx_WritePage(4, (uint8_t *)data);
  if (success) {
    Serial.print("Paquete enviado: ");
    Serial.println(data);
  } else {
    Serial.println("Error enviando paquete. Reintentando...");
  }
}
