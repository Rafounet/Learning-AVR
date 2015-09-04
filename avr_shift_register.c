/*
 * avr_shift_register.c
 *
 * Created: 17/12/2014 16:47:24
 *  Author: Pedro
<<<<<<< HEAD
=======
 * Update:  13/08/2015 00:05:30
 *
 * Exodus 15:18
>>>>>>> 34376f87f1212cef87efec8217a25947f22fde8f
 */ 
#define F_CPU 16000000
#include <avr/io.h>
#include <util/delay.h>

#define DATA  		(_BV(PB0)) 
#define CLOCK 		(_BV(PB1))
#define LATCH 		(_BV(PB2))
#define working 	(_BV(PB3))

void send_byte(char b);

uint8_t bytes;
uint8_t k;
uint8_t shifted = 0b10000000;
uint8_t h;

int main(void){
	DDRB |= CLOCK | DATA | LATCH | working;
<<<<<<< HEAD
	PORTB |= working;
=======
	PORTB |= working; // For put high !MR pin 10 on 595.
>>>>>>> 34376f87f1212cef87efec8217a25947f22fde8f
    while(1){
		for(h=0;h<255;h++){
			send_byte(bytes);
			_delay_ms(200);
			bytes++;
		}
	}
}

void send_byte(char b){
	shifted = 0b10000000;
	for (k=0;k<8;k++){
		PORTB &=~CLOCK;
		
		PORTB = (b & shifted)?(PORTB|DATA):(PORTB&(~DATA)); 
		
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
