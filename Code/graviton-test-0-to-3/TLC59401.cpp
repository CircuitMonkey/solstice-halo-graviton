

#include "Arduino.h"
#include "TLC59401.h"
#define NOP __asm__ __volatile__ ("nop\n\t")

TLC59401::TLC59401(int mode, int blank, int gsClk, int xLat, int sClk, int sIn ) {
    _modePin = mode;
    _blankPin = blank;
    _gsClkPin = gsClk;
    _xLatPin = xLat;
    _sClkPin = sClk;
    _sInPin = sIn;

    //outputs[] = { 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0 };

}

void TLC59401::begin() {
    // Set BLANK initially high (outputs off)
    pinMode( _blankPin, OUTPUT );
    digitalWrite( _blankPin, HIGH );

    // No support for GS mode yet. So set MODE high.
    pinMode( _modePin, OUTPUT );
    digitalWrite( _modePin, HIGH );

    // 
    pinMode( _gsClkPin, OUTPUT );
    digitalWrite( _gsClkPin, LOW );
    
    // 
    pinMode( _xLatPin, OUTPUT );
    digitalWrite( _xLatPin, LOW );
    
    // 
    pinMode( _sClkPin, OUTPUT );
    digitalWrite( _sClkPin, LOW );
    
    // 
    pinMode( _sInPin, OUTPUT );
    digitalWrite( _sInPin, LOW );


    // TO DO Pre fill the GS register!
    
    
}

/**
 * Set the numbered channel to this PWM value.
 */
void TLC59401::set( int channel, int value ) {
  outputs[channel] = value;
}

/** 
 *  Push the buffer into the chip.
 */
void TLC59401::push() {
    digitalWrite( _xLatPin, LOW );
    blank(HIGH);

    // Clock out each bit in the buffers (at 12-bits MSB to LSB)
    // Also MS Byte is first too.
    for ( int i=15; i>=0; i-- ) {
      int v = (outputs[i]>>6) & 0x3f; // truncate to 6 bits
      for( int j=5; j>=0; j-- ) {
        digitalWrite(_sInPin, (v&(1<<j))>>j);
        digitalWrite(_sClkPin, HIGH);
        NOP;
        digitalWrite(_sClkPin, LOW);
        
      }
    }
    digitalWrite(_sInPin, LOW);

    // latch it
    digitalWrite( _xLatPin, HIGH );
    NOP;
    digitalWrite( _xLatPin, LOW );

    blank(LOW);
    for ( int ck=0; ck<4096; ck++ ) {
      digitalWrite(_gsClkPin, HIGH);
      NOP;
      digitalWrite(_gsClkPin, LOW );
    }
//    blank(HIGH);
}

void TLC59401::blank( int blanked) {
  digitalWrite( _blankPin, blanked );  
}
