//#define __AVR_DEV_LIB_NAME__ tn1614
//#define __AVR_ATtiny1614__
#define F_CPU (20000000UL / 6)

#include <avr/io.h>
#include <util/delay.h>

int main() {
  // Blink on PB3 (physical 6)

  //PORTB.DIRSET = PIN3_bm;
  VPORTB.DIR |= PIN3_bm;
    
  while (true) {
    //PORTB.OUTSET = PIN3_bm;
    VPORTB.OUT |= PIN3_bm;
    _delay_ms(1000);

    //PORTB.OUTCLR = PIN3_bm;
    VPORTB.OUT &= ~PIN3_bm;
    _delay_ms(1000);
  }
  return 0;
}
