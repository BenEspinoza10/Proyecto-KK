void calculo() {

  cronometro = millis();
  int vibratemp=0;
  //ucrono = micros();
  while (flag_rolling == 1) {
    //Serial.print(millis()); Serial.print(";");
    //Serial.print("Vibra: ");
    //Serial.println(digitalRead(IN_INTERRUPT));

    //Leer y filtrar puerto del IR, si es ruido salir    
    huella = MeasureTurnCount(T_FILTER, IN_LINE);  //Sensa durante 1.0 mS para filtrar ruido
    if(huella==-1){
      //aqui hay que hacer otra cosa en vez de retornar
      continue;
    }
    huellatemp = huella;

    if (huella1temp != huellatemp and (huellatemp!=-1 or huella1temp!=-1)) {
      //Serial.print("Sensor: ");
      //Serial.println(bool(huella));
      vueltas_temp++;
      dT_line=millis()-cronometro;
      escritura_SD_temp();
      if(dT_line>timeout_dTmax) timeout_dTmax = dT_line; // tiempo más alto en que se detecta una transición de linea
      
      cronometro = millis();
      //ucrono = micros();
      huella1temp = huellatemp;
      vibra=1;
      led_green_set(huella); 
    }
    
    if (MeasureDigitalN(3, IN_INTERRUPT)==0 && vibra==1){ 
      //si no está vibrando
      //if (vibratemp==0) vibra=0;
      vibratemp++;
    }
    
    if ( millis()-cronometro>timeout_halada*1000  || (vibratemp>5 &&  millis()-cronometro > timeout_dTmax*2) ) {
      //fin evento halada de papel, si timeout (por precaución) o si deja de vibrar y además dT es más largo q el máximo registrado en la halada 
      //se hacen los cálculos respectivos, actualmente timeout halada está definido con 3 segundos      
      vueltas_temp = vueltas_temp / n_octocoplador * 0.5;
      vueltas_totales += vueltas_temp; //OJO esta variable parece q no es necearia, verificar ELIMINAR
      halada++; //OJO esta variable parece q no es necearia, verificar ELIMINAR

      //aqui hay que poner el cálculo de los metros gastados-usados      
      //radio_temp = (80 -distancia_rollo(MeasureAnalogN(SAMPLES,IN_DIAMETER)));
      sensorDiametro = MeasureAnalogN(SAMPLES,IN_DIAMETER);
      diametro = distancia_rollo(sensorDiametro);
      gasto_temp = M_PI * diametro * vueltas_temp;
      dT_line=555555;
      //escritura de datos en la tarjeta SD
      //print_temporal_tirada();
      led_green_set(not(huella));
      escritura_SD();
      vueltas_temp=0;
      gasto_temp=0;
      flag_rolling=0;
      timeout_dTmax=0;
      vibra=0;
      
      //blink_led_blue(2,100);
      led_blue_set(not(huella));
      delay(200);
      led_off();
      sensorDiametro2 = MeasureAnalogN(SAMPLES,IN_DIAMETER);
      
    }    
    
    //Serial.println(millis());
  }

  
}

double distancia_rollo(double lectura){
  double x = lectura*0.76;
  //double d = -6.73+62.6*x-25.2*pow(x,2)+3.79*pow(x,3);
  double d = 539.0*pow(x,-0.359)/1000.0; // en metros
  return d;
}