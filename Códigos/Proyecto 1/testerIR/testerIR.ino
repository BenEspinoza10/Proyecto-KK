void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(4,OUTPUT); //Se pone el pin en escritura
  digitalWrite(4,LOW);
}

void loop() { 
  // put your main code here, to run repeatedly:
  Serial.print(analogRead(A1));
  Serial.print("\t");
  /*Serial.print(analogRead(A2));
  Serial.print("\t");
  Serial.print(analogRead(A3));
  Serial.print("\t");
  Serial.print(analogRead(A4));
  Serial.print("\t");
  Serial.print(analogRead(A5));
  Serial.print("\t");*/
  Serial.println(analogRead(A7));
  delay(300);
}
