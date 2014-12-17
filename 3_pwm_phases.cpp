/*
 * 3_pwm_phase.cpp
 *
 * Created: 30/11/2014 18:02:00
 *  Author: Pedro
 */ 
#define F_CPU 16000000UL
#include <avr/io.h>
#include <stdbool.h>
#include <util/delay.h>

bool up9, up10, up11;
uint8_t pin9 = 0;
uint8_t pin10 = 85;
uint8_t pin11 = 170;
void setPWM(unsigned pin9, unsigned pin10, unsigned pin11){
		OCR1A = pin9;
		OCR1B = pin10;
		OCR2A = pin11;
}
int main (void) {
	DDRB |= _BV(PB3) | _BV(PB2) | _BV(PB1);

	DDRD |= _BV(PD5) | _BV(PD6) | _BV(PB7);
	
	TCCR1A |= _BV(COM0A1) | _BV(COM0B1)| _BV(WGM00);
	TCCR2A |= _BV(COM0A1) | _BV(WGM00);
	TCCR1B |= _BV(CS00);
	TCCR2B |= _BV(CS00);
	while (1) {
		setPWM(pin9,pin10,pin11);
		//9
		pin9 += up9 ? 1 : -1;
		if (pin9 == 255)
		up9 = false;
		else if (pin9 == 0)
		up9 = true;
		//10
		pin10 += up10 ? 1 : -1;
		if (pin10 == 255)
		up10 = false;
		else if (pin10 == 0)
		up10 = true;
		//11
		pin11 += up11 ? 1 : -1;
		if (pin11 == 255)
		up11 = false;
		else if (pin11 == 0)
		up11 = true;
		_delay_us(40);
		//amarelo #5
		if ((pin10 >= pin11) & (pin10 >= pin9)){
			PORTD |= _BV(PD5);
		} 
		else{
			PORTD &= ~(1 << PD5);
		}
		
		//azul #7
		if ((pin11 >= pin10) & (pin11 >= pin9)){
			PORTD |= _BV(PD6);
		}
		else{
			PORTD &= ~(1 << PD6);
		}
		
		//vermelho #6
		if ((pin9 >= pin11) & (pin9 >= pin10)){
			PORTD |= _BV(PD7);
		}
		else{
			PORTD &= ~(1 << PD7);
		}
	}
}
