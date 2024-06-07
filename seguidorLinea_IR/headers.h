#define IN_INTERRUPT 2       //PIN PARA EL SENSOR DE VIBRACIÓN (INTERRUPT)
#define OUT_DISABLE_POWER 4  //PIN QUE CONTROLA VDD
#define IN_DIAMETER A7       //PIN PARA EL SENSOR INFRARROJO DE DISTANCIA
#define IN_LINE A6           //PIN PARA EL SENSOR INFRARROJO QUE CUENTA LAS VUELTAS(este es para analógico)
//#define IN_LINE 6             //digital

#define SAMPLES 10  //No. Muestras del sensor analogo
#define T_FILTER 1 //mSeg sensando el sensor de línea

//pines para la luz led
#define LED_RED 9
#define LED_BLUE 10
#define LED_GREEN 11

#define PUSH_BUTTON 7 //pin para el boton