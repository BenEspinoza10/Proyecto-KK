void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
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
  //Serial.print(analogRead(A7));
  Serial.println("");
  delay(300);
}
