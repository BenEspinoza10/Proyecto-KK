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
  double ar=double(cum);
  int n=1;

  if (cum >= umbral) {
    cum = 1;
  } else {
    cum = 0;
  }
  
  while (millis() - lapse < sensingTime) {
    int temp=analogRead(analogPort);
    n++;
    ar=ar*(n-1.0)/n+double(temp)/n;  //promedia y acumula cada muestra
    
    if (temp >= umbral) {
      temp = 1;
    } else {
      temp = 0;
    }
    if (cum != temp) return -1;
  }
  sensorLinea=int(ar); // retorna el promedio análogo y digital, solo si pasó el filtro
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