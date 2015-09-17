/*
*	Just for fun
*	atmega32
*	Dó, Ré, Mí, Fá
*/

#define F_CPU 8000000

#include <avr/io.h>
#include <util/delay.h>

/*
	Freq. Calc.
	
	Fpwm = F_clock / (2*n*Top)
	
	Top = F_clock / (2*n*Fpwm)
*/

//		note		Top
#define do_		15151
#define re_		13468
#define mi_		12121
#define fa_		11363
#define sol_	10101
#define la_		9090
#define si_		8080


void set_pwm(uint16_t i, uint16_t o);

int main (void) {
	DDRD |= (1 << PD4); // PD4 as out
	TCCR1A |= (1 << COM1B1); // Enable OC1B
	TCCR1B |= (1 << WGM13) | (1 << CS10); // Set PWM phase and freq. n = 1
	ICR1 = 1000;
	for(;;){
	//-*-
		set_pwm(do_,500);
		_delay_ms(500);
		
		set_pwm(re_,500);
		_delay_ms(500);
		
		set_pwm(mi_,500);
		_delay_ms(500);
		
		set_pwm(fa_,500);
		_delay_ms(250);
		set_pwm(fa_,0);
		_delay_ms(50);
		
		set_pwm(fa_,500);
		_delay_ms(250);
		set_pwm(fa_,0);
		_delay_ms(50);
		
		set_pwm(fa_,500);
		_delay_ms(500);
		
		//----------------------------------
		set_pwm(do_,500);
		_delay_ms(300);
		
		set_pwm(re_,500);
		_delay_ms(250);
		
		set_pwm(do_,500);
		_delay_ms(200);
		
		set_pwm(re_,500);
		_delay_ms(200);
		set_pwm(re_,0);
		_delay_ms(200);
		
		set_pwm(re_,500);
		_delay_ms(200);
		set_pwm(re_,0);
		_delay_ms(200);
		
		set_pwm(re_,500);
		_delay_ms(400);
		set_pwm(re_,0);
		_delay_ms(400);
		
		//----------------------------------
		
		set_pwm(do_,500);
		_delay_ms(220);
		
		set_pwm(sol_,500);
		_delay_ms(220);
		
		set_pwm(fa_,500);
		_delay_ms(220);
		
		set_pwm(mi_,500);
		_delay_ms(180);
		set_pwm(mi_,0);
		_delay_ms(100);
		
		set_pwm(mi_,500);
		_delay_ms(180);
		set_pwm(mi_,0);
		_delay_ms(100);
		
		set_pwm(mi_,500);
		_delay_ms(300);
		
		//--------------
		
		set_pwm(do_,500);
		_delay_ms(220);
		
		set_pwm(re_,500);
		_delay_ms(220);
		
		set_pwm(mi_,500);
		_delay_ms(220);
		
		set_pwm(fa_,500);
		_delay_ms(1000);
		
		set_pwm(do_,0);
		_delay_ms(2000);
		
	//-*-
	}
}


void set_pwm(uint16_t i, uint16_t o){
	ICR1 = i;
	OCR1B = o;	
}
