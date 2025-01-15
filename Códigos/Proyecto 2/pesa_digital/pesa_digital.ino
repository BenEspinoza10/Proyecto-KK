//Incluye las librerías
#include "HX711.h"
#include <Wire.h>
#include <EEPROM.h>

HX711 balanza;
const int zero = 3;
int DT = 4;
int CLK = 5;
int peso_calibracion = 182; // Es el peso referencial a poner, en mi caso mi celular pesa 185g (SAMSUMG A20)
long escala = 0.01; //escala para que lea con precisión de 0.01 gramos
int state_zero = 0;
int last_state_zero = 0;
//Función calibración
void calibration() { // despues de hacer la calibracion puedes borrar toda la funcion "void calibration()"
  boolean conf = true;
  long adc_lecture;
  // restamos el peso de la base de la balaza
 
  Serial.print("Calibrando base balanza");
  delay(3000);
  balanza.read();
  balanza.set_scale(escala); //La escala por defecto es 1
  balanza.tare(20);  //El peso actual es considerado zero.
  //Iniciando calibración
  while (conf == true) {
    Serial.print("Peso referencial:");
    Serial.print(peso_calibracion);
    Serial.print(" g        ");
    delay(3000);
    Serial.print("Ponga el Peso");
    Serial.print(" referencial");
    delay(3000);
    //Lee el valor del HX711
    adc_lecture = balanza.get_value(100);
    //Calcula la escala con el valor leido dividiendo el peso conocido
    escala = adc_lecture / peso_calibracion;
    //Guarda la escala en la EEPROM
    EEPROM.put( 0, escala );
    delay(100);
    Serial.println("Retire el Peso Referencial");
    delay(3000);
    Serial.println("READY!!....");
    delay(3000);
    conf = false; //para salir de while
  }
}
void setup() {
  Serial.begin(9600);
    while (! Serial);
    Serial.println("Proyecto KKs con Pesa Digital v1");
  
  balanza.begin(DT, CLK);//asigana los pines para el recibir el trama del pulsos que viene del modulo
  pinMode(zero, INPUT);//declaramos el pin2 como entrada del pulsador
  pinMode(13,OUTPUT);
  
  EEPROM.get( 0, escala );//Lee el valor de la escala en la EEPROM
  if (digitalRead(zero) == 1) { //esta accion solo sirve la primera vez para calibrar la balanza, es decir se presionar ni bien se enciende el sistema
    calibration();
  }
  calibration();
  balanza.set_scale(escala); // Establecemos la escala
  balanza.tare(20);  //El peso actual de la base es considerado zero.
}
void loop() {
  //int state_zero = digitalRead(zero);
  float peso;
  peso = balanza.get_units(10);  //Mide el peso de la balanza
  //Muestra el peso
  
  Serial.print("Peso: ");
  Serial.print(peso, 2);
  Serial.println(" g        ");
  delay(5);
  //Botón de zero, esto sirve para restar el peso de un recipiente 
  /*if ( state_zero != last_state_zero) {
    if (state_zero == LOW) {
      balanza.tare(10);  //El peso actual es considerado zero.
    }
  }*/
  //last_state_zero  = state_zero;
  //if (peso>=500)digitalWrite(13,1);
  //else if(peso<=500)digitalWrite(13,0);
}
