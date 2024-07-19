#define IN_INTERRUPT 2       //PIN PARA EL SENSOR DE VIBRACIÓN (INTERRUPT)
#define OUT_DISABLE_POWER 4  //PIN QUE CONTROLA VDD
#define IN_DIAMETER A7       //PIN PARA EL SENSOR INFRARROJO DE DISTANCIA
#define IN_LINE A1       //PIN PARA EL SENSOR INFRARROJO QUE CUENTA LAS VUELTAS(este es para analógico)

#define SAMPLES 10  //No. Muestras del sensor analogo
#define T_FILTER 1 //mSeg sensando el sensor de línea

//pines para la luz led
#define LED_RED 5
#define LED_GREEN 6
#define LED_BLUE 7

#define PUSH_BUTTON 9 //pin para el boton

#define VAR_INIT -55555
#define T_AVG_HALADA 340 //(ms)Tiempo promedio de Dt entre lineas de una misma halada
#define T_MAX_HALADA 1200 //(ms)Tiempo máximo de Dt entre lineas de una misma halada
#define T_SLEEP 3.0 //Tiempo de espera para mdormir y medir diametro

#define WRITE_AUX 1 //escribir o no el CSV auxiliar
#define ENABLE_SD 1 // Habilitar SD con 1 o Desabilitar con 0 para desarrollo o debugg
#define PRINT 0

#define T_CONFIG 1000 // Tiempo de espera para configurar 5000 usualmente

