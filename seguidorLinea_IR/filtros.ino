#include "headers.h"

int MeasureDigitalN(unsigned int samplesNumber, int digitalPort) {
  int cum = digitalRead(digitalPort);

  for (int i = 0; i < samplesNumber; i++) {
    if (cum != digitalRead(digitalPort)) return -1;
  }
  return cum;
}

int MeasureDigitalT(unsigned int sensingTime, int digitalPort) {
  unsigned long lapse = millis();
  int cum = digitalRead(digitalPort);
  while (millis() - lapse < sensingTime) {
    if (cum != digitalRead(digitalPort)) return -1;
  }
  return cum;
}

int MeasureTurnCount(unsigned int sensingTime, int analogPort) {
  unsigned long lapse = millis();
  int cum = analogRead(analogPort);
  float analog=cum;
  
  if (cum >= umbral) {
    cum = 1;
  } else {
    cum = 0;
  }
  while (millis() - lapse < sensingTime) {
    int temp;
    if (analogRead(analogPort) >= umbral) {
      temp = 1;
    } else {
      temp = 0;
    }
    if (cum != temp) return -1;
  }
  sensorLinea = float(analog/1023.0);
  return cum;
}

// Entrega valor de la lectura análoga entre 0 y 1023, filtrada y promediando varias lecturas
float MeasureAnalogN(unsigned int samplesNumber, int analogPort) {
  float sum = 0;
  for (int i = 0; i < samplesNumber; i++) {
    sum += analogRead(analogPort);
  }
  return (sum / samplesNumber);
}