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
  Serial.println("PN532 inicializado en modo emisor.");
}

void loop() {
  File myFile = SD.open("a.csv");
  if (!myFile) {
    Serial.println("Error al abrir el archivo CSV.");
    delay(5000);
    return;
  }

  // Enviar marcador de inicio
  enviarMarcador("START");

  // Leer y enviar datos en bloques
  uint8_t packetID = 0;
  while (myFile.available()) {
    char buffer[4];
    memset(buffer, 0, sizeof(buffer)); // Limpiar el buffer
    for (int i = 0; i < 4 && myFile.available(); i++) {
      buffer[i] = myFile.read();
    }

    // Enviar paquete con ID
    enviarPaquete(buffer, packetID);
    packetID++;
  }

  // Enviar marcador de fin
  enviarMarcador("END");

  Serial.println("Transferencia completada.");
  myFile.close();

  delay(10000);  // Esperar 10 segundos antes de intentar una nueva transferencia
}

void enviarMarcador(const char *marcador) {
  bool enviado = false;

  while (!enviado) {
    uint8_t success = nfc.ntag2xx_WritePage(4, (uint8_t *)marcador);
    if (success) {
      delay(50);
      uint8_t ack[16];
      nfc.ntag2xx_ReadPage(5, ack);
      if (strncmp((char *)ack, "ACK", 3) == 0) {
        enviado = true;
      } else {
        Serial.println("Reenviando marcador...");
      }
    } else {
      Serial.println("Error enviando marcador. Reintentando...");
    }
  }
}

void enviarPaquete(const char *data, uint8_t id) {
  char packet[16];
  memset(packet, 0, sizeof(packet));
  snprintf(packet, sizeof(packet), "%03d%s", id, data); // Incluir ID del paquete

  bool enviado = false;

  while (!enviado) {
    uint8_t success = nfc.ntag2xx_WritePage(4, (uint8_t *)packet);
    if (success) {
      delay(50);
      uint8_t ack[16];
      nfc.ntag2xx_ReadPage(5, ack);
      if (strncmp((char *)ack, "ACK", 3) == 0) {
        enviado = true;
        Serial.print("Paquete ");
        Serial.print(id);
        Serial.println(" enviado correctamente.");
      } else {
        Serial.println("Reenviando paquete...");
      }
    } else {
      Serial.println("Error enviando paquete. Reintentando...");
    }
  }
}
