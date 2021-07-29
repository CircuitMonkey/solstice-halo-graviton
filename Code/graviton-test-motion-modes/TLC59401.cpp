

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


    // Set GS defaults
    // Mode LOW, 192 bits
    digitalWrite( _modePin, LOW );
    digitalWrite( _xLatPin, LOW );
    digitalWrite( _sInPin, HIGH);
    for ( int i=191; i>=0; i-- ) {
      digitalWrite(_sClkPin, HIGH);
      NOP;
      digitalWrite(_sClkPin, LOW);
    }
    digitalWrite( _xLatPin, HIGH );  // Latch
    NOP;
    digitalWrite( _xLatPin, LOW );
    

    // Set DC defaults.
    // Mode HIGH, 96-bits
    digitalWrite( _modePin, HIGH );
    digitalWrite( _xLatPin, LOW );
    digitalWrite( _sInPin,  LOW);
    for ( int i=95; i>=0; i-- ) {
      digitalWrite(_sClkPin, HIGH);
      NOP;
      digitalWrite(_sClkPin, LOW);
    }
    digitalWrite( _xLatPin, HIGH );  // Latch
    NOP;
    digitalWrite( _xLatPin, LOW );
    
    digitalWrite( _modePin, LOW );
    
    digitalWrite( _blankPin, LOW );
    
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
    digitalWrite( _modePin, HIGH );   // DC Mode  96-bits
    digitalWrite( _xLatPin, LOW );
    blank(HIGH);

    // Clock out each bit in the buffers (at 12-bits MSB to LSB)
    // Also MS Byte is first too.
    for ( int i=15; i>=0; i-- ) {
      int v = (outputs[i]>>6) & 0x3f; // truncate to 6 bits
      for( int j=5; j>=0; j-- ) {
        digitalWrite(_sInPin, (v&(1<<j))>>j);
        //digitalWrite(_sInPin, HIGH);
        digitalWrite(_sClkPin, HIGH);
        //NOP;
        digitalWrite(_sClkPin, LOW);
        //NOP;
      }
    }
    digitalWrite(_sInPin, LOW);

    // latch it
    digitalWrite( _xLatPin, HIGH );
    //NOP;
    digitalWrite( _xLatPin, LOW );
    //NOP;
    blank(LOW);
    pulse();
//    blank(HIGH);
}

void TLC59401::pulse() {
  blank(HIGH);
  NOP;
  blank(LOW);
  NOP;
    for ( int ck=0; ck<4096; ck++ ) {
      digitalWrite(_gsClkPin, HIGH);
      //NOP;
      digitalWrite(_gsClkPin, LOW );
      //NOP;
    }
  blank(HIGH);
}

void TLC59401::blank( int blanked) {
  digitalWrite( _blankPin, blanked );  
}
