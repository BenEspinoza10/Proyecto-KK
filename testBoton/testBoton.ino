void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(9,INPUT_PULLUP); //Se pone el pin en escritura
  
}

void loop() {
  Serial.println(digitalRead(9));

}
