#include <SPI.h>
#include <SD.h>
#include <SoftwareSerial.h>

// Pines para SoftwareSerial
#define BT_RX 4  // Pin RX del HC-05
#define BT_TX 5  // Pin TX del HC-05

// Pin para el módulo SD
#define SD_CS_PIN 10  // Pin CS de la tarjeta SD

SoftwareSerial bluetooth(BT_RX, BT_TX);  // Crear objeto SoftwareSerial para Bluetooth
File myFile;

void setup() {
  // Configurar comunicación serial para debug y Bluetooth
  Serial.begin(115200);     // Comunicación Serial USB para debug
  bluetooth.begin(9600);  // Comunicación Serial con el HC-05
  Serial.println("Inicializando...");

  // Inicializar la tarjeta SD
  if (!SD.begin(10)) {
    Serial.println("Error al inicializar la tarjeta SD.");
    while (1)
      ;  // Detener si no se encuentra la SD
  }
  Serial.println("Tarjeta SD inicializada.");
}

void loop() {
  // Abrir el archivo CSV
  if (bluetooth.available()>0) {
    myFile = SD.open("a.csv");
    if (myFile) {
      Serial.println("Enviando datos al Bluetooth...");
      bluetooth.println("Iniciando transferencia de archivo...");

      // Leer y enviar el contenido del archivo por Bluetooth
      while (myFile.available()) {
        char data = myFile.read();
        bluetooth.write(data);  // Enviar datos al HC-05
        Serial.write(data);     // También mostrar en el monitor serial para debug
      }

      myFile.close();  // Cerrar el archivo
      bluetooth.println("\nTransferencia completada.");
      Serial.println("\nTransferencia completada.");
    } else {
      Serial.println("No se pudo abrir el archivo CSV.");
      bluetooth.println("Error: No se pudo abrir el archivo CSV.");
    }
    delay(5000);  // Esperar 5 segundos antes de intentar de nuevo    
  }
}
