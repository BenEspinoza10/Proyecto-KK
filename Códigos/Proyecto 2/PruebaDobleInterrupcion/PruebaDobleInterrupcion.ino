// **** INCLUDES *****
#include "LowPower.h"

// Use pin 2 as wake up pin
const int wakeUpPin2 = 2;
const int wakeUpPin3 = 3;

void wakeUp2()
{
    // Just a handler for the pin interrupt.
    Serial.println("Desperté con boton 2");
}

void wakeUp3()
{
    // Just a handler for the pin interrupt.
    Serial.println("Desperté con boton 3");
}


void setup()
{
    // Configure wake up pin as input.
    // This will consumes few uA of current.
    pinMode(wakeUpPin2, INPUT);   
    pinMode(wakeUpPin3, INPUT);   
    Serial.begin(9600);
}

void loop() 
{
    // Allow wake up pin to trigger interrupt on low.
    attachInterrupt(0, wakeUp2, HIGH);
    attachInterrupt(1, wakeUp3, HIGH);
    
    // Enter power down state with ADC and BOD module disabled.
    // Wake up when wake up pin is low.
    LowPower.powerDown(SLEEP_FOREVER, ADC_OFF, BOD_OFF); 
    delay(100);
    
    // Disable external pin interrupt on wake up pin.
    detachInterrupt(0); 
  
}
