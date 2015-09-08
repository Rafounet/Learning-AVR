#define F_CPU 16000000

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define BAUD 9600
#define MYUBRR F_CPU/16/BAUD-1
#define led _BV(PB5) // Eu estava usando antes para 'debuggar' o código.

volatile char rx; // Uso para ter certeza que que estou conectado. Ao enviar um caractere o AVR o envia de volta.

void usart_init();
void adc_init();
void send_char(char c);

volatile uint8_t adcl, adch; // Variáveis para armazenas os 10 bits do ADC.

int main (void) {
	DDRB |= led; // Está ai. Não é necessário mais.
	cli(); // Desabilita as interrupções.
	adc_init(); // Inicia o ADC
	usart_init(); // Iniciar a USART
	sei(); // Habilita as interrupções.
   while(1){
   	send_char(adch); // Envia os 2 'high' bits 
   	send_char(adcl); // Envia os 8 'low' bits
   	send_char(0x0a); // Quebra linha
   	_delay_ms(60000);// 1m de delay.
   	_delay_ms(60000);// 1m de delay.
   	_delay_ms(60000);// 1m de delay.
   	_delay_ms(60000);// 1m de delay.
   	_delay_ms(60000);// 1m de delay.
   	_delay_ms(60000);// 1m de delay.
   	_delay_ms(60000);// 1m de delay.
   	_delay_ms(60000);// 1m de delay.
   	_delay_ms(60000);// 1m de delay.
   	_delay_ms(60000);// 1m de delay.
   } 
}

ISR (USART_RX_vect){
	rx = UDR0; // Guarda o valor lido na serial.
	UDR0 = rx; // O envia novamente.
}

ISR (ADC_vect){
	adcl = ADCL; // Guarda os 8 'low' bits
	adch = ADCH; // Guarda os 2 'high' bits
}

void adc_init(void){
	ADMUX |= _BV(MUX0); // Usar ADC1.
	ADMUX |= _BV(REFS0); // Usar AREF como referência.
	ADCSRA |= _BV(ADEN) | _BV(ADIE) | _BV(ADATE); // Liga o ADC, habilita as intettupções.
	ADCSRA |= (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); // Prescaler para 16Mhz.
	ADCSRB = 0x00; // "free-running mode".
	ADCSRA |= _BV(ADSC);
}

void usart_init(void){
	UBRR0H = (MYUBRR >> 8); // Configura o BAUD rate.
   UBRR0L = MYUBRR;			//		||		 ||		||
   
   UCSR0B |= _BV(RXEN0) | _BV(TXEN0); // Habilita o TX e o RX da serial.
   UCSR0C |= _BV(UCSZ01) | _BV(UCSZ00); // Define a comunicação como 1 byte, 1 stop bit.
	UCSR0B |= _BV(RXCIE0); // Habilita as interrupções causadas pelo RX.
}

void send_char(char c){
	while((UCSR0A & _BV(UDRE0)) == 0){} // Espera UDR0 estar livre.
	UDR0 = c;
}
