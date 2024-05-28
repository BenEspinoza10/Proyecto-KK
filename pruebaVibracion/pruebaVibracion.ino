void setup() {
  // put your setup code here, to run once:
  digitalRead(2); //interrupt para el sensor de vibración
  Serial.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println(digitalRead(2));
}
