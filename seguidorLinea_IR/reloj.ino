void reloj_setup(){
  Wire.begin();
  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    //while (1) delay(10);
    return;
  }
  const unsigned int buildTimeEEPROMaddress = 1000;
  const char storedBuildTime[9];
  EEPROM.get(buildTimeEEPROMaddress, storedBuildTime);
  if (strcmp(__TIME__, storedBuildTime) == 0) {
    Serial.println("This is not the first run after upload");
  } else {
    Serial.println("This is the first run after upload");
    EEPROM.put(buildTimeEEPROMaddress, __TIME__);
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    Serial.println("Real Time Clock initialized");
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