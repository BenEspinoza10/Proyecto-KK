void calculo() {

  unsigned long cronometro_v=millis();
  unsigned long timeout_dTmax=T_AVG_HALADA;
  double dt_avg=0;
  cronometro = millis();

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
      //calcula dT de la línea anterior
      dT_line=millis()-cronometro;

      //fin_halada = (dT_line>T_MAX_HALADA || millis()-cronometro_v > T_MAX_HALADA);
      fin_halada = ( dT_line>timeout_dTmax*1.5 || dt_avg>T_AVG_HALADA*3 || dT_line>T_MAX_HALADA || millis()-cronometro_v > T_MAX_HALADA);
      
      if(!fin_halada){ // si gira, vibra y el Dt de giro fue corto
        if(ENABLE_SD) escritura_SD_temp();  //solo escribe SD si hace parte de la misma halada
        if(dT_line>timeout_dTmax) timeout_dTmax = dT_line; // tiempo más alto en que se detecta una transición de linea
        //Promediar dT 
        dt_avg = dt_avg*(vueltas_temp-1.0)/vueltas_temp + double(dT_line)/vueltas_temp;  //promedia y acumula cada muestra

        Serial.print("Dt; ");
        Serial.print(dT_line);
        Serial.print("; dtmax: ");
        Serial.print(timeout_dTmax);
        Serial.print("; dtavg: ");
        Serial.println(dt_avg);
      }

      cronometro = millis();
      huella1temp = huellatemp;
      led_green_set(huella); 
     
      
    }
    
    if (fin_halada) {
      escribe_fin_halada();
    }else if(millis()-cronometro > T_SLEEP*1000 /*&& millis()-cronometro_v > T_SLEEP*1000*/){
      // Tras T_SLEEP segundos de inactividad lee sensor de distancia, escribe en SD y se va a mimir
      sensorDiametro2 = MeasureAnalogN(SAMPLES,IN_DIAMETER);
      escribe_fin_halada();
    }
  }
}


void escribe_fin_halada(){
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
  
  if(ENABLE_SD) escritura_SD();
  else print_temporal_tirada();

  vueltas_temp = 0;
  gasto_temp = 0;
  flag_rolling = 0;
  sensorDiametro2=0;
  blink_led_blue(2,100); 

}

double distancia_rollo(double lectura){
  double x = lectura*0.76;
  //double d = -6.73+62.6*x-25.2*pow(x,2)+3.79*pow(x,3);
  double d = 539.0*pow(x,-0.359)/1000.0; // en metros
  return d;
}