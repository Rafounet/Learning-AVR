#include <avr/io.h>
#include "carrinho.h"

// -----------------------------------------------------------

/*
void send_trigger(void)
{
		PORTB &= ~(1<<PB0);
		_delay_us(1);
		PORTB |= (1<<PB0);
		_delay_us(10);
		trigger_unlock = 1;
		PORTB &= ~(1<<PB0);
}
*/

unsigned char USART_Receive( void ){
	while ( !(UCSRA & (1<<RXC)) );
	return UDR;
}

void USART_Transmit( unsigned char __data){
	while ( !( UCSRA & (1<<UDRE)) );
	UDR = __data;
}
// ---
/*
void ADC_Init(void){
	ADMUX  = (1<<REFS0) | (1<<ADLAR); // AVcc
	ADCSRA |= (1<<ADPS2) | (1<<ADPS1); // Prescaler = 64; ADC enable;
	ADCSRA |= (1<<ADEN);
}
*/
void USART_Init(uint16_t my_ubrr)
{
	UBRRL = my_ubrr;
	UBRRH = my_ubrr>>8;
	UCSRB |= (1<<RXEN) | (1<<TXEN) | (1<<RXCIE);
	UCSRC |= (1<<URSEL) | (1<<UCSZ1) | (1<<UCSZ0); //8bit, 1 stop bit, none.
}

void Timer2_Init(void){
	TCCR2	|= (1<<CS20);
	TIMSK	|= (1<<TOIE2); // Timer overflow interrupt enable.
	ASSR	|= (1<<AS2);
}

void Timer1_Init(void){
	 // PWM 50Hz
	TCCR1A	|= (1<<COM1A1) | (1<<COM1B1) | (1<<WGM11);
	TCCR1B	|= (1<<CS11) | (1<<WGM13) | (1<<WGM12); // Fast PWM, ICR1, Prescaler = 8.
	ICR1 = 19999; // 50Hz;
}

void INT_Settings(void){
	// INT0 conf. Borda de subida e descida
	MCUCR	|= (1<<ISC00);
	GICR	|= (1<<INT0);
}
