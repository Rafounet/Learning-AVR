#define F_CPU 8000000

#include <avr/io.h>
#include <util/delay.h>
#include "lcd.h"
#include <avr/interrupt.h>

#define BAUD 38400
#define MYUBRR F_CPU/BAUD/16 -1

void USART_Transmit(unsigned char data);
void USART_Init(unsigned int ubrr);

unsigned char usart_receive(void);

void get_GPGGA(void);
void get_GPVTG(void);

uint8_t GPGGA_lock, gps_temp, count_j, latitude_n_s, longitude_w_e;

uint8_t gps_time[10], latitude[10], longitude[10], sat_numb[5], DOP[5], alt[8];

int main (void) {

	DDRD |= (1 << PD4);
	cli();
	lcd_init(LCD_DISP_ON_CURSOR);
	USART_Init(MYUBRR);
	TCCR1A |= (1 << COM1B1);
	TCCR1B |= (1 << WGM13) | (1 << CS10);
	ICR1 = 1000;
	OCR1B = 0;
	sei();
	lcd_clrscr();
	while(1){
		
		get_GPGGA();
		
		// #1
		
		// TIME /*
		for(count_j=0;count_j<=1;count_j++){
			lcd_putc(gps_time[count_j]);
		}
		lcd_putc(':');
		
		for(count_j=2;count_j<=3;count_j++){
			lcd_putc(gps_time[count_j]);
		}
		lcd_putc(':');

		for(count_j=4;count_j<=9;count_j++){
			lcd_putc(gps_time[count_j]);
		}
		
		lcd_puts(" GMT");
		// TIME */
		
		lcd_gotoxy(0,1);
		
		lcd_puts("SATELITES: ");
		for(count_j=0;sat_numb[count_j]!=',';count_j++){
			lcd_putc(sat_numb[count_j]);
		}
		_delay_ms(4500);
		lcd_clrscr();
		
		// #2
		
		for(count_j=0;count_j<=1;count_j++){
			lcd_putc(latitude[count_j]);
		}
		
		lcd_putc(223);
		
		for(count_j=2;count_j<=9;count_j++){
			lcd_putc(latitude[count_j]);
		}
		lcd_putc(latitude_n_s);
		
		lcd_gotoxy(0,1);
		
		for(count_j=0;count_j<=1;count_j++){
			lcd_putc(longitude[count_j]);
		}
		
		lcd_putc(223);
		
		for(count_j=2;count_j<=9;count_j++){
			lcd_putc(longitude[count_j]);
		}
		lcd_putc(longitude_w_e);
		
		
		_delay_ms(4500);
		lcd_clrscr();
		
		// #3
		
		lcd_puts("DOP: ");
		for(count_j=0;DOP[count_j]!=',';count_j++){
			lcd_putc(DOP[count_j]);
		}
		
		lcd_gotoxy(0,1);
		
		lcd_puts("Alt: ");
		for(count_j=0;alt[count_j]!=',';count_j++){
			lcd_putc(alt[count_j]);
		}
		
		_delay_ms(2500);
		lcd_clrscr();
	}
}

void get_GPGGA(void){
	GPGGA_lock = 1;
	while(GPGGA_lock){
		gps_temp = usart_receive();
		if(gps_temp == '$'){
		gps_temp = usart_receive();
			if(gps_temp == 'G'){
			gps_temp = usart_receive();
				if(gps_temp == 'P'){
					gps_temp = usart_receive();
					if(gps_temp == 'G'){
						gps_temp = usart_receive();
						if(gps_temp == 'G'){
							gps_temp = usart_receive();
							if(gps_temp == 'A'){
								gps_temp = usart_receive();
								if(gps_temp == ','){
									// Get time
									gps_temp = usart_receive();
									gps_time[0] = gps_temp;
									for(count_j=1;gps_temp != ',';count_j++){
										gps_temp = usart_receive();
										gps_time[count_j] = gps_temp;									
									}
									// Get latitude
									gps_temp = usart_receive();
									latitude[0] = gps_temp;
									for(count_j=1;gps_temp != ',';count_j++){
										gps_temp = usart_receive();
										latitude[count_j] = gps_temp;								
									}
									// N/S
									gps_temp = usart_receive();
									latitude_n_s = gps_temp;
									// Jump ,
									gps_temp = usart_receive();gps_temp = usart_receive();
									// Get longitude
									gps_temp = usart_receive();
									longitude[0] = gps_temp;
									for(count_j=1;gps_temp != ',';count_j++){
										gps_temp = usart_receive();
										longitude[count_j] = gps_temp;								
									}
									// W/E
									gps_temp = usart_receive();
									longitude_w_e = gps_temp;
									// jump 1,
									gps_temp = usart_receive();gps_temp = usart_receive();gps_temp = usart_receive();
									// Sat
									gps_temp = usart_receive();
									sat_numb[0] = gps_temp;
									for(count_j=1;gps_temp != ',';count_j++){
										gps_temp = usart_receive();
										sat_numb[count_j] = gps_temp;
									}
									// DOP
									gps_temp = usart_receive();
									DOP[0] = gps_temp;
									for(count_j=1;gps_temp!=',';count_j++){
										gps_temp = usart_receive();
										DOP[count_j] = gps_temp;
									}
									// Alt
									gps_temp = usart_receive();
									alt[0] = gps_temp;
									for(count_j=1;gps_temp!=',';count_j++){
										gps_temp = usart_receive();
										alt[count_j] = gps_temp;
									}
									GPGGA_lock = 0;
								}
							}
						}
					}
				}		
			}
		}
	}
}

unsigned char usart_receive( void ){
	while ( !(UCSRA & (1<<RXC)) );
	return UDR;
}

void USART_Init( unsigned int ubrr){
	UBRRH = (unsigned char)(ubrr>>8);
	UBRRL = (unsigned char)ubrr;

	UCSRB |= (1<<RXEN)|(1<<TXEN);
	UCSRC |= (1<<URSEL)|(1<<UCSZ1)|(1<<UCSZ0);
}

void USART_Transmit( unsigned char data ){
	while ( !( UCSRA & (1<<UDRE)) );
	UDR = data;
}
