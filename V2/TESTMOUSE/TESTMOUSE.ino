#define SCLK                A5
#define SDIO                A4
#define RESET               5
#define NCS                  4

#define PRODUCT_ID          0x00 // should be 0x12
#define PRODUCTID2          0x3e
#define REVISION_ID         0x01
#define DELTA_Y_REG         0x04
#define DELTA_X_REG         0x03
#define SQUAL_REG           0x05
#define MAXIMUM_PIXEL_REG   0x08
#define MINIMUM_PIXEL_REG   0x0a
#define PIXEL_SUM_REG       0x09
#define PIXEL_DATA_REG      0x0b
#define SHUTTER_UPPER_REG   0x06
#define SHUTTER_LOWER_REG   0x07
#define RESET		    0x3a
#define CPI500v		    0x00
#define CPI1000v	    0x01
#define MOTION 0x02

#define NUM_PIXELS          361

byte pix[360];

void setup()
{
  Serial.begin(57600);

  pinMode(SDIO, OUTPUT);
  pinMode(SCLK, OUTPUT);

  pinMode(RESET, OUTPUT);
  digitalWrite(RESET, LOW);
  
  sync();
  
  ADNS_write(RESET, 0x5a);
  delay(50); // From NRESET pull high to valid mo tion, assuming VDD and motion is present.

}

void loop()
{
  digitalWrite(RESET, LOW);
  delay(50);
  //checkMotion();
  Serial.println(ADNS_read(PRODUCT_ID), BIN);
  //pixelGrab();
  //Serial.println(dx());
}

void sync() {
  pinMode(NCS, OUTPUT);
  digitalWrite(NCS, LOW);
  delayMicroseconds(2);
  digitalWrite(NCS, HIGH);
}

void ADNS_write(unsigned char addr, unsigned char data) {
  char temp;
  int n;

  digitalWrite(NCS, LOW);//nADNSCS = 0; // select the chip

  temp = addr;
  digitalWrite(SCLK, LOW);//SCK = 0;					// start clock low
  pinMode(SDIO, OUTPUT);//DATA_OUT; // set data line for output
  for (n=0; n<8; n++) {
    digitalWrite(SCLK, LOW);//SCK = 0;
    pinMode(SDIO, OUTPUT);
    delayMicroseconds(1);
    if (temp & 0x80)
      digitalWrite(SDIO, HIGH);//SDOUT = 1;
    else
      digitalWrite(SDIO, LOW);//SDOUT = 0;
    temp = (temp << 1);
    digitalWrite(SCLK, HIGH);//SCK = 1;
    delayMicroseconds(1);//delayMicroseconds(1);			// short clock pulse
  }
  temp = data;
  for (n=0; n<8; n++) {
    digitalWrite(SCLK, LOW);//SCK = 0;
    delayMicroseconds(1);
    if (temp & 0x80)
      digitalWrite(SDIO, HIGH);//SDOUT = 1;
    else
      digitalWrite(SDIO, LOW);//SDOUT = 0;
    temp = (temp << 1);
    digitalWrite(SCLK, HIGH);//SCK = 1;
    delayMicroseconds(1);			// short clock pulse
  }
  delayMicroseconds(20);
  digitalWrite(NCS, HIGH);//nADNSCS = 1; // de-select the chip
}

byte ADNS_read(unsigned char addr) {
  byte temp;
  int n;

  digitalWrite(NCS, LOW);//nADNSCS = 0;				// select the chip
  temp = addr;
  digitalWrite(SCLK, OUTPUT); //SCK = 0;					// start clock low
  pinMode(SDIO, OUTPUT); //DATA_OUT;					// set data line for output
  for (n=0; n<8; n++) {

    digitalWrite(SCLK, LOW);//SCK = 0;
    pinMode(SDIO, OUTPUT); //DATA_OUT;
    if (temp & 0x80) {
      digitalWrite(SDIO, HIGH);//SDOUT = 1;
    } 
    else {
      digitalWrite(SDIO, LOW);//SDOUT = 0;
    }
    temp = (temp << 1);
    delayMicroseconds(1);
    digitalWrite(SCLK, HIGH); //SCK = 1;
    delayMicroseconds(1);			// short clock pulse
  }
  
  temp = 0; // This is a read, switch to input
  pinMode(SDIO, INPUT); //DATA_IN;
  for (n=0; n<8; n++) {		// read back the data
    digitalWrite(SCLK, LOW);
    if(digitalRead(SDIO)) {// got a '1'
      temp |= 0x1;
    }
    if( n != 7) temp = (temp << 1); // shift left
    digitalWrite(SCLK, HIGH);
  }
  
  digitalWrite(NCS, HIGH);// de-select the chip
  return temp;
}

int dx() { 

  byte x = ADNS_read(DELTA_X_REG);
  
  if( x == 0 ) return 0;

  int xv = (int16_t)x;
  if (x & 0x80) {					// negative value
    x = ~x + 1;
    xv = (0 - (int16_t)x);
  }
  return xv;
}

int dy() { 

  byte y = ADNS_read(DELTA_Y_REG);
  
  if( y == 0 ) return 0;

  int yv = (int16_t)y;
  if (y & 0x80) {					// negative number
    y = ~y + 1;
    yv = (0 - (int16_t)y);
  }

  return yv;
}

void checkMotion(){
  byte m = ADNS_read(MOTION);
  Serial.println(m,BIN);
}