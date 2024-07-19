void calculo() {

  unsigned long cronometro_v=millis();
  cronometro = millis();
  timeout_dTmax=T_AVG_HALADA;

  while (flag_rolling == 1) {

    // Leer sensor de vibración, 1 si vibra, 0 si no está vibrando
    vibra=MeasureDigitalN(SAMPLES, IN_INTERRUPT);
    if (vibra==1) {
      cronometro_v = millis(); // si aún está vibrando reiniciar medición de tiempo de vibración
    }

    //Leer y filtrar puerto del IR, si es ruido salir    
    huella = MeasureTurnCount(T_FILTER, IN_LINE);  //Sensa durante 1.0 mS para filtrar ruido
    if(huella==-1){
      //aqui hay que hacer otra cosa en vez de retornar
      continue;
    }
    huellatemp = huella;
    gira=0;
    bool fin_halada=0;
    if (huella1temp != huellatemp and (huellatemp!=-1 or huella1temp!=-1)) {
      gira=1;
      vueltas_temp++;
      dT_line=millis()-cronometro;
      fin_halada = (dT_line>T_MAX_HALADA || millis()-cronometro_v > T_MAX_HALADA);
      if(!fin_halada){ // si gira, vibra y el Dt de giro fue corto
        escritura_SD_temp();  //solo escribe acá si hace parte de la misma halada
        //Promediar dT 
        //if(dT_line>timeout_dTmax) timeout_dTmax = dT_line; // tiempo más alto en que se detecta una transición de linea
      }

      cronometro = millis();
      huella1temp = huellatemp;
      led_green_set(huella); 
     
      Serial.print(dT_line);
      Serial.print(" dt; ");
    }
    
    if (fin_halada || millis() - cronometro > timeout_halada * 1000) {
      sensorDiametro2=sensorDiametro;
      escribe_SD_fin_halada();
    }
  }
}


void escribe_SD_fin_halada(){
  //fin evento halada de papel, se hacen los cálculos respectivos, actualmente está definido con 2 segundos      
  vueltas_temp = vueltas_temp / n_octocoplador * 0.5;
  vueltas_totales += vueltas_temp; //OJO esta variable parece q no es necearia, verificar ELIMINAR
  halada++; //OJO esta variable parece q no es necearia, verificar ELIMINAR

  //aqui hay que poner el cálculo de los metros gastados-usados      
  //radio_temp = (80 -distancia_rollo(MeasureAnalogN(SAMPLES,IN_DIAMETER)));
  sensorDiametro = MeasureAnalogN(SAMPLES,IN_DIAMETER);
  diametro = distancia_rollo(sensorDiametro);
  gasto_temp = M_PI * diametro * vueltas_temp;

  //escritura de datos en la tarjeta SD
  //print_temporal_tirada();
  escritura_SD();
  vueltas_temp = 0;
  gasto_temp = 0;
  flag_rolling = 0;

  Serial.println(dT_line);
  Serial.print(" dt; ");
 
  blink_led_green(2,200); 

}

double distancia_rollo(double lectura){
  double x = lectura*0.76;
  //double d = -6.73+62.6*x-25.2*pow(x,2)+3.79*pow(x,3);
  double d = 539.0*pow(x,-0.359)/1000.0; // en metros
  return d;
}