#define F_CPU 16000000

#include <avr/io.h> 
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdbool.h>

#define BAUD 9600
#define MYUBRR F_CPU/16/BAUD-1
#define led _BV(PB5)
#define bot _BV(PD2)

volatile char rx;
volatile bool e;
void send_char(char c);
void send_string(char s[]);

int main (void) {
	DDRB |= led;
	DDRD &= ~bot;
	PORTD |= _BV(PORTD2);
	cli();
	UBRR0H = (MYUBRR >> 8);
  	UBRR0L = MYUBRR;
   
  	UCSR0B |= _BV(RXEN0) | _BV(TXEN0);
  	UCSR0C |= _BV(UCSZ01) | _BV(UCSZ00);
	UCSR0B |= _BV(RXCIE0);
	
	EICRA |= _BV(ISC01);
	EIMSK |= _BV(INT0);
	
	PORTB &= ~led;
	sei();
   while(1){
   	if(e){
   		cli();
			if(PORTB != 0b00100000){
   			send_string("1");
   			
   			PORTB |= led;
   		}
   		else if(PORTB == 0b00100000){
   			send_string("0");
   			
   			PORTB &= ~led;
   		}
   		sei();
   		e = false;	
   	}
   } 
}

ISR (USART_RX_vect){
	rx = UDR0;
	if(rx==0x31){
   	send_string("on");
   	PORTB |= led;
   }
   else if(rx==0x30){
   	send_string("off");
   	PORTB &= ~led;
   }
   else if(rx==0x23){
   	PORTB ^= led;
   	_delay_ms(200);
   	PORTB ^= led;
   	_delay_ms(200);
   	PORTB ^= led;
   	_delay_ms(200);
   	PORTB ^= led;
   	_delay_ms(200);
   }
}

ISR (INT0_vect){
	_delay_ms(300);
	e = true;
}

void send_char(char c){
	while((UCSR0A & _BV(UDRE0)) == 0){}
	UDR0 = c;
}

void send_string(char s[]){
	int i = 0;
	while(s[i] != 0b00000000) {
		send_char(s[i]);
		i++;
	}
}
