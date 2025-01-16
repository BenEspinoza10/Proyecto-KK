void laser_setup(){
  Wire.begin();

  laser.setTimeout(500);
  if (!laser.init())
  {
    Serial.println(F("Failed to detect and initialize sensor!"));
    while (1) {}
  } 
  laser.setMeasurementTimingBudget(100000);  //esto es para mejorar la precisión (100ms)
}

//Funcion que retorna la distancia detectada por el sensor de distancia láser
//En caso de que la lectura sea inválida, retorna un -1
int leerLaser() {
  int lectura = laser.readRangeSingleMillimeters();

  if (laser.timeoutOccurred()) { 
    return -1;
  }else{
    return lectura;
  }  
}