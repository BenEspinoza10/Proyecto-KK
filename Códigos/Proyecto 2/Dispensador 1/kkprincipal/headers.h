//PINES

#define HALL 2       //PIN PARA EL SENSOR DE EFECTO HALL (INTERRUPT)
#define OPEN_SENSOR 3  //PIN PARA EL SENSOR DE APERTURA DEL DISPOSITIVO (INTERRUPT)

//BLUETOOTH
#define BT_RX 4  // SE CONECTA AL TXD DEL BLUETOOTH
#define BT_TX 5  // SE CONECTA AL RXD DEL BLUETOOTH

//pines para la luz led
#define LED_RED 6
#define LED_GREEN 7
#define LED_BLUE 8

#define OUT_DISABLE_POWER 9 //PIN PARA CONTROLAR LA CORRIENTE HACIA EL BLUETOOTH


//CONTROL TODO:REVISAR CUALES SE USAN
#define VAR_INIT -55555
#define T_AVG_HALADA 340 //(ms)Tiempo promedio de Dt entre lineas de una misma halada
#define T_MAX_HALADA 1200 //(ms)Tiempo máximo de Dt entre lineas de una misma halada
#define T_SLEEP 3.0 //Tiempo de espera para mdormir y medir diametro

#define WRITE_AUX 0 //escribir o no el CSV auxiliar
#define ENABLE_SD 1 // Habilitar SD con 1 o Desabilitar con 0 para desarrollo o debugg
#define PRINT 0

#define T_CONFIG 2000 // Tiempo de espera para configurar 5000 usualmente

#define SAMPLES 10  //No. Muestras del sensor analogo
#define T_FILTER 1 //mSeg sensando el sensor de línea

//NOTAS
//pines que no puedo tocar A4, A5(RTC), 11,12,13,10 (SD)