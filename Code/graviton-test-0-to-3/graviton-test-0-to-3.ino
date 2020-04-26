/*
 
 */
#include "TLC59401.h"
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

Adafruit_NeoPixel strip0 = Adafruit_NeoPixel(4, PIXELS0, NEO_GRB + NEO_KHZ800);

int potValue0 = 0;  // variable to store the value coming from the sensor
int potValue1 = 0;  // variable to store the value coming from the sensor
int potValue2 = 0;  // variable to store the value coming from the sensor
int potValue3 = 0;  // variable to store the value coming from the sensor

void setup() {
//  Serial.begin(9600);
//  while (!Serial) {}

  vibes.begin();
  vibes.push();
  vibes.blank(0);
  
  strip0.begin();
  strip0.show(); // Initialize all pixels to 'off'
}

void loop() {
  // read the value from the sensor:
  potValue0 = analogRead(POT0);
  potValue1 = analogRead(POT1);
  potValue2 = analogRead(POT2);
  potValue3 = analogRead(POT3);

//  strip0.setPixelColor(0, Wheel(potValue0/4) );
//  strip0.setPixelColor(1, Wheel(potValue1/4) );
//  strip0.setPixelColor(2, Wheel(potValue2/4) );
//  strip0.setPixelColor(3, Wheel(potValue3/4) );
  strip0.setPixelColor(0, potValue0/4, 0, 0 );
  strip0.setPixelColor(1, potValue1/4, 0, 0 );
  strip0.setPixelColor(2, potValue2/4, 0, 0 );
  strip0.setPixelColor(3, potValue3/4, 0, 0 );
  strip0.show();

  vibes.set( 0, potValue0<<2 );  // Shift 10-bit analog read to 12-bit stored value.
  vibes.set( 1, potValue1<<2 );
  vibes.set( 2, potValue2<<2 );
  vibes.set( 3, potValue3<<2 );
  
  vibes.push();
  
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
    return strip0.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return strip0.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip0.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}
