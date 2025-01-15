void setup() {
  // put your setup code here, to run once:
  pinMode(2,INPUT); //interrupt para el sensor de vibración
  Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println(digitalRead(2));
}
