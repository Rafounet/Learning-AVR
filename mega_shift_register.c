/*
 * mega_shift_register.c
 *
 * Created: 17/12/2014 16:47:24
 *  Author: Pedro
 * pin 10 of 74595 to vcc and 13 to gnd
 * 
 *   		< Mt 6:34 >
 *
 */ 

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>

#define DATA  (_BV(PB4))
#define CLOCK (_BV(PB5))
#define LATCH (_BV(PB6))
#define funcionando (_BV(PB7))

void send_byte(char b);

uint8_t bytes;

int main(void){
	DDRB |= CLOCK | DATA | LATCH | funcionando;
	PORTB |= funcionando;
    while(1){
		for(uint8_t h=0;h<255;h++){
			send_byte(bytes);
			_delay_ms(200);
			bytes++;
		}
	}
}

void send_byte(char b){
	uint8_t shifted = 0b10000000;
	for (uint8_t k=0;k<8;k++){
		PORTB &=~CLOCK;
		PORTB = (b & shifted)?(PORTB|DATA):(PORTB&(~DATA));
		if (b & shifted){
			PORTB |= DATA;
		} 
		else{
			PORTB &=~DATA;
		}
		_delay_loop_1(1);
		PORTB |= CLOCK;
		_delay_loop_1(1);
		PORTB &=~DATA;
		shifted = shifted >> 1;
		}
	PORTB |= LATCH;
	_delay_loop_2(0);
	PORTB &= ~LATCH;
	_delay_loop_2(0);
}
