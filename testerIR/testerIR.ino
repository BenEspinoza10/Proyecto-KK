void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(4,OUTPUT); //Se pone el pin en escritura
  pinMode(6,INPUT);
  digitalWrite(6,LOW);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.print(digitalRead(6));
  Serial.print("\t");
  Serial.println(analogRead(A7));
}
