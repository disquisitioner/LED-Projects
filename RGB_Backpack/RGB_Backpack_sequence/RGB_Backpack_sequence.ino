#include <Wire.h>
#include <RGB_Matrix_Arduino.h>

#define BKG 0x440000
#define DOT 0x0000ff

uint8_t buf[192];

RGB_Matrix matrix = RGB_Matrix((uint8_t)0x7f, buf);

void setup() {
  
  uint8_t x, y;
  uint8_t r,g,b;
  
  matrix.begin();
  
  randomSeed(analogRead(0));
  for(y=0;y<8;y++){
    r = random(255); g = random(255); b = random(255);
    for(x=0;x<8;x++) {
      matrix.setPixel(x,y,r,g,b);
    }
  }
  matrix.transmitBuf();
}

void loop() {
  uint8_t x, y;
  uint8_t r,g,b;
  
  for(x=0;x<8;x++) {
    for(y=0;y<7;y++) {
      buf[(x*24)+(y*3)]     = buf[(x*24)+((y+1)*3)];
      buf[(x*24)+(y*3) + 1] = buf[(x*24)+((y+1)*3) + 1];
      buf[(x*24)+(y*3) + 2] = buf[(x*24)+((y+1)*3) + 2];
    }
  }
  r = random(255); g = random(255); b = random(255);
  for(x=0;x<8;x++){
    matrix.setPixel(x,7,r,g,b);
  }
  matrix.transmitBuf();
  delay(200);
}
  
