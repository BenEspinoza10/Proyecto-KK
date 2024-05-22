#define encoder1 2
#define encoder2 3
int contador = 0;
void setup() {
  // put your setup code here, to run once:
  pinMode(encoder1,INPUT);
  pinMode(encoder2,INPUT);
  attachInterrupt(digitalPinToInterrupt(encoder1), encoderinfrarrojo1, CHANGE);
  attachInterrupt(digitalPinToInterrupt(encoder2), encoderinfrarrojo2, CHANGE);
  Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
  /*Serial.print(digitalRead(encoder1));
  Serial.print("::");
  Serial.println(digitalRead(encoder2));*/
  
}

void encoderinfrarrojo1() {
  if (digitalRead(encoder1) == HIGH) {
    if (digitalRead(encoder2) == LOW) {
      contador--;
      Serial.println(contador);
    } else {
      contador++;
      Serial.println(contador);
    }
  } else {
    if (digitalRead(encoder2) == LOW) {
      contador++;
      Serial.println(contador);
    } else {
      contador--;
      Serial.println(contador);
    }
  }
}

void encoderinfrarrojo2() {
  if (digitalRead(encoder2) == HIGH) {
    if (digitalRead(encoder1) == LOW) {
      contador++;
    } else {
      contador--;
    }
  } else {
    if (digitalRead(encoder1) == LOW) {
      contador--;
    } else {
      contador++;
    }
  }
}
