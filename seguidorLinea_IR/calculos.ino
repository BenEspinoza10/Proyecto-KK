void calculo() {

  cronometro = millis();
  while (flag_rolling == 1) {
    //Leer y filtrar puerto del IR, si es ruido salir
    //TODO: hay que cambiar esto a digital
    huella = MeasureTurnCount(T_FILTER, IN_LINE);  //Sensa durante 1.0 mS para filtrar ruido
    huellatemp = huella;

    if (huella1temp != huellatemp) {
      //Serial.print("Sensor: ");
      //Serial.println(bool(huella));
      vueltas_temp++;
      cronometro = millis();
      //escritura_SD_temp();
      huella1temp = huellatemp;
    }

    if (millis() - cronometro > timeout_halada * 1000) {
      //fin evento halada de papel, se hacen los cálculos respectivos, actualmente está definido con 2 segundos      
      vueltas_temp = vueltas_temp / n_octocoplador * 0.5;
      vueltas_totales += vueltas_temp;
      halada++;

      //aqui hay que poner el cálculo de los cm usados      
      radio_temp = (8 -distancia_rollo(analogRead(IN_DIAMETER)));
      //sensorRadio = radio_max * radio_temp;
      gasto_temp = 2 * M_PI * radio_temp * vueltas_temp;


      //escritura de datos en la tarjeta SD
      //escritura_SD();
      blink_led_green();      
      print_temporal_tirada();

      vueltas_temp = 0;
      gasto_temp = 0;
      flag_rolling = 0;
    }
  }
}


double distancia_rollo(int lectura){
  double d = -6.73+62.6*lectura-25.2*pow(lectura,2)+3.79*pow(lectura,3);
  return d;
}