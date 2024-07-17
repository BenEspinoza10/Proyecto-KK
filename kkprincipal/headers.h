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

#define WRITE_AUX 0 //escribir o no el CSV auxiliar