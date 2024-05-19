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

float MeasureAnalogN(unsigned int samplesNumber, int analogPort) {
  //digitalWrite(OUT_DISABLE_POWER, LOW); // HABILITAR CORRIENTE PARA SENSORES
  //delay(10); // Delay para esperar que el sensor de distancia esté activo
  float sum = 0;
  for (int i = 0; i < samplesNumber; i++) {
    sum += analogRead(analogPort) / 1023.0;
  }
  //digitalWrite(OUT_DISABLE_POWER, HIGH); // DESABILITAR CORRIENTE PARA SENSORES
  return (sum / samplesNumber);
}