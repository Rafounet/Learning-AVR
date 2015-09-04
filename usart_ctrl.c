/*
* My first USART code.
* Pedro 04/09/2015
*
*/

#define F_CPU 16000000 // Clock speed 16MHz

#include <avr/io.h> 
#include <avr/interrupt.h>

#define BAUD 9600
#define MYUBRR F_CPU/16/BAUD-1
#define led _BV(PB5)

volatile char rx;

void send_char(char c);
void send_string(char s[]);

int main (void) {
	DDRB |= led;
	cli(); // Disable interrupts
	UBRR0H = (MYUBRR >> 8);  // Set baud rate
   UBRR0L = MYUBRR;			 // ||   ||   ||
   
   UCSR0B |= _BV(RXEN0) | _BV(TXEN0); // Enable USART receiver and transmitter
   UCSR0C |= _BV(UCSZ01) | _BV(UCSZ00); // Set frame: 8data, 1stop bit
	UCSR0B |= _BV(RXCIE0); // Enable RX interrupt
	sei(); // Enable interrupts
   while(1){} 
}

ISR (USART_RX_vect){ // Receive interrupt
	rx = UDR0;
	if(rx==0x31){ // if rx = "1"
   	send_string("on");
   	send_char(0xa); // New line
   	PORTB |= led; // Turn-on PB5
   }
   else if(rx==0x30){ // if rx = "0"
   	send_string("off");
   	send_char(0xa); // New line
   	PORTB &= ~led; // Turn-off PB5
   }
}

void send_char(char c){
	while((UCSR0A & _BV(UDRE0)) == 0){} // Wait UDR0
	UDR0 = c;
}

void send_string(char s[]){
	int i = 0;
	while(s[i] != 0b00000000) {
		send_char(s[i]);
		i++;
	}
}
















