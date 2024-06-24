#include "headers.h"

void reloj_setup() {
  Wire.begin();
  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    
    while (true){
      blink_led_red(3, 200);
      blink_led_yellow(2, 200);
      delay(10000);
    }
    return;
  }

  //Código para setear la hora del reloj sólo si no estaba seteada de antes.
  if (!rtc.isrunning()) {
    Serial.println("RTC is NOT running, let's set the time!");
    // When time needs to be set on a new device, or after a power loss, the
    // following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    Serial.println("Real Time Clock initialized");
  }
}