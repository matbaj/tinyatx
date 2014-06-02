/* Name: TinyAtx
   Architecture: ATtiny85
 */
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>

uint8_t turned_on = 0;
uint8_t g=0;
uint8_t timer = 0;


ISR(WDT_vect)
{
  cli();
  if (turned_on == 1)
  { 
    if(++timer>60)
    {
      timer = 0;
      PORTB &= (~(1)<<PB0); 
       _delay_ms(500);
      PORTB |= (1<<PB0);
    } 
  }
  sei();
  return;
}


ISR(PCINT0_vect)
{
  cli();

  if ((PINB & (1<<PB2)) ^ (1<<PB2))
  {
    _delay_ms(200);
    if (turned_on == 1) 
    {
      timer = 0;
    }
  }
  if ((PINB & (1<<PB5))^ (1<<PB5))
  {
    _delay_ms(200);
    if (turned_on == 1) 
    {
      OCR0B = 0;
      g = 0;
      turned_on = 0;
      timer = 0;
      
      PORTB &= (~(1)<<PB0);
    } 
    else 
    {
      PORTB |= (1<<PB0);
      turned_on = 1;
    }
  }
  sei();
  return;
} 
 
int main(void)
{ 
  
  uint8_t sine[91] = {0, 1, 2, 3, 5, 8, 11, 15, 20, 25, 30, 36, 43, 49, 56, 64, 72, 80, 88, 97, 105, 114, 123, 132, 141, 150, 158, 167, 175, 183, 191, 199, 206, 212, 219, 225, 230, 235, 240, 244, 247, 250, 252, 253, 254, 255, 254, 253, 252, 250, 247, 244, 240, 235, 230, 225, 219, 212, 206, 199, 191, 183, 175, 167, 158, 150, 141, 132, 123, 114, 105, 97, 88, 80, 72, 64, 56, 49, 43, 36, 30, 25, 20, 15, 11, 8, 5, 3, 2, 1, 0};
  
  PORTB = 0b00100100;
  DDRB  = 0b00000011;

  TCCR0A =  2<<COM0B0 | 3<<WGM00;
  TCCR0B = 0<<WGM02 |1<<CS00;
  TCCR1 = 1<<CS10;
  GTCCR = 1<<PWM1B | 2<<COM1B0;

  PCMSK |= (1<<PCINT5); 
  PCMSK |= (1<<PCINT2); 
  MCUCR |= 1<<ISC00; 
  GIMSK |= (1<<PCIE);

  WDTCR |= (_BV(WDCE) | _BV(WDE));
  WDTCR =   _BV(WDIE) | 
            _BV(WDP2) | _BV(WDP1);
  sei();

  OCR0B = sine[45];
  _delay_ms(1000);
  OCR0B = sine[0];

  while(1){
    if (turned_on == 1)
    {
      if (g==45) {_delay_ms(3000);}
      if (++g>90) {_delay_ms(2000);g=0;}
      if (turned_on == 1) OCR0B = sine[g];
    }
    _delay_ms(50);
  }
  return 0;
}
