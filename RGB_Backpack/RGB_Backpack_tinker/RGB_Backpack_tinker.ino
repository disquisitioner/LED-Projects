#include <Wire.h>
#include <RGB_Matrix_Arduino.h>

#define BKG 0x440000
#define DOT 0x0000ff

uint8_t buf[192];

RGB_Matrix matrix = RGB_Matrix((uint8_t)0x7f, buf);

uint8_t cnt[8][8];
uint8_t x, y;

void setup() {


  Serial.begin(9600);
  matrix.begin();
  
  randomSeed(analogRead(0));
  
  // Initialize matrix as our display
  initmatrix();
  
  // Update display
  matrix.transmitBuf();
}


void loop() {
  /*
   * Erase current dot, which means draw it in its current
   * count color.
   */
  matrix.setPixel(x,y,(BKG|cnt[x][y]));
  
  // Generate new dot location. 
  movedot();
  matrix.setPixel(x,y,DOT);
  
  /*
   * Increment cell visit counter.  Once any cell gets to 100 we
   * zero them all and start over (just because...)
   */
  if(cnt[x][y] >= 100) {
    initmatrix();
  }
  else {
    cnt[x][y]++;
  }
 
  // Update display
  matrix.transmitBuf();
  
  // Pause for effect...
  delay(1000);
}

// Initialize to background color and zero counters
void initmatrix() {
  uint8_t i, j;
  for(i=0;i<8;i++) {
    for(j=0;j<8;j++) {
      matrix.setPixel(i,j,BKG);
      cnt[i][j] = 0;
    }
  }
  newdot();
}

// Place a new dot somewhere in a blank screen
void newdot() {
  x = 2 + random(5);
  y = 2 + random(5);
  matrix.setPixel(x,y,DOT);
  if(cnt[x][y] >= 0xff) cnt[x][y] = 1;  // This better never be true...
  else                  cnt[x][y]++;
}

void movedot()
{
  uint8_t nx, ny;
  
  // Handle corners (2 moves)
  if(x == 0 && y == 0) {
    if(random(2) == 0) {x = 1; y = 0;}
    else               {x = 0; y = 1;}
    return;
  }
  if(x == 0 && y == 7) {
    if(random(2) == 0) {x = 1; y = 7;}
    else               {x = 0; y = 6;}
    return;
  }
  if(x == 7 && y == 0) {
    if(random(2) == 0) {x = 6; y = 0;}
    else               {x = 7; y = 1;}
    return;
  }
  if(x == 7 && y == 7) {
    if(random(2) == 0) {x = 7; y = 6;}
    else               {x = 6; y = 7;}
    return;
  }
  
  // Handle non-corner edges (3 moves)
  if(x == 0) {
    nx = random(3);
    if(nx == 0)      {y--;}
    else if(nx == 1) {x++;}
    else             {y++;}
    return;
  }
  if(y == 0) {
    nx = random(3);
    if(nx == 0)      {x--;}
    else if(nx == 1) {y++;}
    else             {x++;}
    return;
  }
  if(x == 7) {
    nx = random(3);
    if(nx == 0)      {y--;}
    else if(nx == 1) {x--;}
    else             {y++;}
    return;
  }
  if(y == 7) {
    nx = random(3);
    if(nx == 0)      {x--;}
    else if(nx == 1) {y--;}
    else             {x++;}
    return;
  }
  
  // Handle out-in-the-open (4 moves)
  nx = random(4);
  if(nx == 0)      {x--;}
  else if(nx == 1) {y++;}
  else if(nx == 2) {x++;}
  else             {y--;}
  return;
}

