void calculo() {

  cronometro = millis();
  while (flag_rolling == 1) {
    //Leer y filtrar puerto del IR, si es ruido salir
    //huella = MeasureDigitalN(SAMPLES, IN_LINE);
    huella = MeasureDigitalT(T_FILTER, IN_LINE);  //Sensa durante 1.0 mS para filtrar ruido
    if (huella < 0) return;

    digitalWrite(LED_BUILTIN, bool(huella));
    huellatemp = huella;

    if (huella1temp != huellatemp) {
      Serial.print("Sensor: ");
      Serial.println(bool(huella));      
      vueltas_temp++;
      escritura_SD_temp();
      cronometro = millis();            
      huella1temp = huellatemp;
    }

    if (millis() - cronometro > timeout_halada * 1000) {
      //fin evento halada de papel, se hacen los cálculos respectivos, actualmente está definido con 2 segundos
      //digitalWrite(LED_BUILTIN, LOW);
      vueltas_temp = vueltas_temp / n_octocoplador * 0.5;
      vueltas_totales += vueltas_temp;
      halada++;

      //aqui hay que poner el cálculo de los cm usados
      // Leer y normalizar sensor IR de diametro: Aprox 0.7 rollo vacío - 0.02 rollo nuevo
      radio_temp = (1 - MeasureAnalogN(SAMPLES, IN_DIAMETER));
      sensorRadio = radio_max * radio_temp;
      gasto_temp = 2 * M_PI * sensorRadio * vueltas_temp;


      //escritura de datos en la tarjeta SD
      escritura_SD();

      Serial.println("%rollo; radio; giro ;  cms ");
      Serial.print(radio_temp * 100, 0);
      Serial.print(" ; ");
      Serial.print(sensorRadio, 2);
      Serial.print(" ; ");
      Serial.print(vueltas_temp, 2);
      Serial.print(" ; ");
      Serial.println(gasto_temp, 1);

      vueltas_temp = 0;
      gasto_temp = 0;
      flag_rolling = 0;
      ID++;
    }
  }
}