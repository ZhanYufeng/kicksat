#include <SpriteRadio.h>
#include <SpriteMag.h>

static unsigned char prn0[64] = {
  0b00000000, 0b01110110, 0b10101101, 0b01010110, 0b00010111, 0b01111010, 0b00111000, 0b10001011,
  0b10010011, 0b10110001, 0b00110001, 0b00100110, 0b00101010, 0b11110111, 0b01010011, 0b01101011,
  0b01011110, 0b11111111, 0b00000110, 0b01000111, 0b01000010, 0b01010010, 0b11101011, 0b11000100,
  0b00001101, 0b00100110, 0b01010011, 0b01001001, 0b11101110, 0b00001110, 0b11101101, 0b11110010,
  0b00000111, 0b10010010, 0b01110100, 0b00010010, 0b10111101, 0b00011000, 0b10001010, 0b00101011,
  0b10101011, 0b10001100, 0b10111110, 0b00001110, 0b00000111, 0b11011101, 0b11101000, 0b00011110,
  0b10011000, 0b01010101, 0b10111000, 0b01101000, 0b01001111, 0b11011111, 0b00111001, 0b01100011,
  0b11001011, 0b10111010, 0b01011111, 0b00100100, 0b11011010, 0b10000000, 0b01010000, 0b10111110
};

static unsigned char prn1[64] = {
  0b11111111, 0b01101110, 0b10000101, 0b10111011, 0b00011100, 0b11111101, 0b00111111, 0b11000110,
  0b11000000, 0b10101111, 0b11101000, 0b11011101, 0b10010011, 0b00111011, 0b01100010, 0b00010010,
  0b11111101, 0b11011111, 0b00010100, 0b11000000, 0b11010001, 0b00100011, 0b00100011, 0b01010100,
  0b10010000, 0b01011001, 0b00000001, 0b01111111, 0b01001110, 0b11000010, 0b01001000, 0b10110111,
  0b00111111, 0b00111100, 0b00100010, 0b00010001, 0b11000101, 0b10010011, 0b11010011, 0b00001110,
  0b00110000, 0b10010110, 0b11000110, 0b00010001, 0b01100010, 0b01100011, 0b11111101, 0b01000000,
  0b01100100, 0b10100001, 0b00100111, 0b01000111, 0b11101111, 0b10000111, 0b10100000, 0b10111101,
  0b01111101, 0b01010000, 0b10001101, 0b11001100, 0b01011000, 0b10011010, 0b10111100, 0b10011100
};

//Initialize the radio class, supplying the Gold Codes that correspond to 0 and 1
SpriteRadio radio = SpriteRadio(prn0, prn1);

SpriteMag mag = SpriteMag();

float declination = -10*(PI/180);

void setup() {
  pinMode(5, OUTPUT);
  radio.txInit();
  mag.init();
  Serial.begin(9600);
}

void loop() {
  //Blink LED while we're doing stuff
  digitalWrite(5, HIGH);
  
  MagneticField b = mag.read();
  
  //Calculate heading in radians from magnetic field components
  float heading = atan2(b.y, b.x);
  
  //correct for declination
  heading += declination;
  
  //Scale between 0 and 2*PI
  if(heading < 0) heading += 2*PI;
  if(heading > 2*PI) heading -= 2*PI;
  
  //Convert to degrees
  heading = heading*(180/PI);
  
  char heading_string[5];
  
  sprintf(heading_string,"%3i",int(heading));
  heading_string[3] = '\n';
  
  Serial.print(heading_string);

  radio.transmit(heading_string,4);
  
  digitalWrite(5,LOW);
  
  delay(500);
}