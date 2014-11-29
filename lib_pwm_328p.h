/*
 * lib_pwm_328p.h
 *
 * Created: 28/11/2014 22:25:59
 *  Author: Pedro
 *
 * It's made only for atmega328p!
 * This is a simple lib, but I very happy because I've made this without any help.
 * You need put this file where are the your libraries, in my case is here:
 * "C:\Program Files\Atmel\Atmel Toolchain\AVR8 GCC\Native\3.4.1056\avr8-gnu-toolchain\avr\include\util"
 * Made it just use!
 *
 * Only the pins 16 and 15 can be used. (pins 9 and 10 on arduino UNO).
 * You can change it, but will also need to change other things the program.
 *
 *
 * I've not found a way to put a delay in minMaxPWM();
 * You need put a delay after minMaxPWM();
 * Eg
 -------------------------------
 #define F_CPU 16000000UL

 #include <util/lib_pwm_328p.h>
 #include <util/delay.h>
 int main(void){
	 pwmPin9();
	 pwmPin10();
	 while (1){
		 minMaxPWM();
		 _delay_ms(1);
	 }
 }
 -------------------------------
 * This isn't the best way to do a PWM in any AVR I think. But works!
 *
 */ 
#include <avr/io.h>
#include <stdbool.h>

 uint8_t pwm;
 bool increment = true;
 void minMaxPWM(void){
	 OCR1A = pwm;
	 OCR1B = pwm;
	 pwm += increment ? 1 : -1;
	 if (pwm == 255){
		 increment = false;
	 }
	 else if (pwm == 0){
		 increment = true;
	 }
 }

 void pwmPin9(void){
	 DDRB |= _BV(PB1);
	 TCCR1A |= _BV(COM0A1) | _BV(WGM00);
	 TCCR1B |= _BV(CS01);
 }

 void pwmPin10(void){
	 DDRB |= _BV(PB2);
	 TCCR1A |= _BV(COM0B1) | _BV(WGM00);
	 TCCR1B |= _BV(CS01);
 }

int function(void)
{
  return 0;
}
