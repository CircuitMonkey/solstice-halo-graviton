/*
    Motion Modes

    - Ring Up-down
    - Column rotate
    - Base bloom
    - Random
    
 */
#include "TLC59401.h"
#include "modes.h"

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

#define BTN1  2
#define BTN2  3

// Halo for Rev. A is incorrect A5 to A0 are reversed
// Rev. A
#define POT0 A5
#define POT1 A4
#define POT2 A3
#define POT3 A2

// Rev. B
//#define POT0 A0
//#define POT1 A1
//#define POT2 A2
//#define POT3 A3

#define PIXELS0 10
#define PIXELS1 11
#define PIXELS2 12
#define PIXELS3 13

// Rev. B
//#define MODE  A4
//#define BLANK A5

// Rev. A
#define MODE  A1
#define BLANK A0

#define GSCLK  9
#define XLAT   8
#define SCLK   6
#define SIN    5

TLC59401 vibes = TLC59401(MODE, BLANK, GSCLK, XLAT, SCLK, SIN );

//Adafruit_NeoPixel strip0 = Adafruit_NeoPixel(4, PIXELS0, NEO_GRB + NEO_KHZ800);
//Adafruit_NeoPixel strip1 = Adafruit_NeoPixel(4, PIXELS1, NEO_GRB + NEO_KHZ800);
//Adafruit_NeoPixel strip2 = Adafruit_NeoPixel(4, PIXELS2, NEO_GRB + NEO_KHZ800);

Adafruit_NeoPixel strip[3] = {
   Adafruit_NeoPixel(4, PIXELS0, NEO_GRB + NEO_KHZ800),
   Adafruit_NeoPixel(4, PIXELS1, NEO_GRB + NEO_KHZ800),
   Adafruit_NeoPixel(4, PIXELS2, NEO_GRB + NEO_KHZ800)
};

int potValue0 = 0;  // variable to store the value coming from the sensor
int potValue1 = 0;  // variable to store the value coming from the sensor
int potValue2 = 0;  // variable to store the value coming from the sensor
int potValue3 = 0;  // variable to store the value coming from the sensor

int mode = 0;
int modeStep = 0;

#define CLOCKS 6
int clk = 0;

void setup() {
  Serial.begin(115200);
  while (!Serial) {}

  vibes.begin();
  vibes.push();
  vibes.blank(0);
  
  strip[0].begin();
  strip[0].show(); // Initialize all pixels to 'off'
  strip[1].begin();
  strip[1].show(); // Initialize all pixels to 'off'
  strip[2].begin();
  strip[2].show(); // Initialize all pixels to 'off'
}

void loop() {
  if ( clk > 0 ) {
    // Pulse gsclk
    vibes.pulse();
  } else {
    doPass();
  }

  if ( clk <= 0 ) {
    clk = CLOCKS;
  }
  clk--;
}

void doPass() {
  // read the value from the sensor:
  potValue0 = analogRead(POT0);
  potValue1 = analogRead(POT1);
  potValue2 = analogRead(POT2);
  potValue3 = analogRead(POT3);


  
//  strip0.setPixelColor(0, potValue0/4, 0, 0 );
//  strip0.setPixelColor(1, potValue1/4, 0, 0 );
//  strip0.setPixelColor(2, potValue2/4, 0, 0 );
//  strip0.setPixelColor(3, potValue3/4, 0, 0 );
//  strip0.show();

  // i == ring
  //Serial.print("Step: ");
  //Serial.println( modeStep );
  
  //Serial.print("r:");
  for ( int i=0; i<3; i++ ) {
    //Serial.print("\t");
    //Serial.print(i);
    //Serial.print(":");
    
    for ( int col=0; col<4; col++ ) {
      int idx = i*4+col;
      strip[i].setPixelColor(col, upDownRing[modeStep][i*4+col], 0, 0 );
      vibes.set( i*4+col, upDownRing[modeStep][i*4+col] << 4);  // Shift 8-bit val to 12-bit stored value.
      //vibes.set( 0, 200 << 2);  // Shift 10-bit analog read to 12-bit stored value.
      //Serial.print("\t");
      //Serial.print( upDownRing[modeStep][i*4+col] );

//      
//      strip[i].setPixelColor(0, upDownRing[modeStep][i*4+0], 0, 0 );
//      strip[i].setPixelColor(1, upDownRing[modeStep][i*4+1], 0, 0 );
//      strip[i].setPixelColor(2, upDownRing[modeStep][i*4+2], 0, 0 );
//      strip[i].setPixelColor(3, upDownRing[modeStep][i*4+3], 0, 0 );
//      strip[i].show();
//
//      vibes.set( i*4+0, potValue0<<2 );  // Shift 10-bit analog read to 12-bit stored value.
//      vibes.set( i*4+1, potValue1<<2 );
//      vibes.set( i*4+2, potValue2<<2 );
//      vibes.set( i*4+3, potValue3<<2 );
    }
    //Serial.println();
    strip[i].show();
    
    //vibes.push();
  }
  //Serial.println();
  vibes.push();

  // GSCLK must be toggled to keep outputs running.
  //delay(900);

  
  modeStep++;
  if ( modeStep > 15 ) {
    modeStep = 0;
    //Serial.println();
  }
  
//  Serial.print( potValue0 );
//  Serial.print( " " );
//  Serial.print( potValue1 );
//  Serial.print( " " );
//  Serial.print( potValue2 );
//  Serial.print( " " );
//  Serial.print( potValue3 );
//  Serial.print( " " );
//  Serial.print("\n");

  //delay(50);
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return strip[0].Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return strip[0].Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip[0].Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}
