/*
 * main.c
 *
 * Autor: Pedro Vinicius
 * Última modificação: 07/03/2016	22:17
 * 
 * ######### Aquivo principal #########
 *
 * Código para o ATMega32 com clock de 8MHz
 *
 * Estou usando um sensor hc-sr04 para medir a distância entre o veículo
 * e os obstáculos. Também estou usando um módulo bluetooth hc-06 para 
 * realizar a comunicação com o computador.
 * 
 *
 * No momento os caracteres usados são w,a,s,d,x,c e f. Onde:
 *	w = move para frente
 *	s = freia/vai para trás
 *	a = vira para a esquerda
 *	d = vira para a direita
 *	x = "centraliza" o PWM do ESC em 7,5%. Valor que seria enviado com o stick no meio
 *	c = faz o mesmo que 'x', mas para o servo que controla a direção.
 *	f = Ativa o modo "autonomo". Por enquanto só se afasta ou se aproxima dos obstáculos
 */
#define F_CPU 8000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "carrinho.h"

uint16_t l_sensor_l;
uint8_t case_f, rr_ok;
volatile uint8_t	timer2_ovf, trigger_unlock, echo_ok, time_to_send;
volatile uint8_t 	move_dir, move_dir_fwd_rr, move_dir_l_r;
volatile uint16_t	total_timer2_inc;

/**  **************** FUNÇÕES *************** **/
void _move_l_r(uint8_t dir_l_r){
	switch(dir_l_r){
	case 'a':
	{
		OCR1B = 1100;
		break;
	}
	case 'd':
	{
		OCR1B = 1850;
		break;
	}
	case 'c':
	{
		OCR1B = 1500;
		break;
	}
	default:
	{
		OCR1B = 1500;
		break;
	}
	}
}

void _move_fwd_rr(uint8_t dir_fwd_rr)
{
	switch(dir_fwd_rr)
	{
	case 'w':
	{
		if(total_timer2_inc < 90){
			USART_Transmit('#');
			USART_Transmit('P');
			USART_Transmit('A');
			USART_Transmit('R');
			USART_Transmit('E');
			USART_Transmit(0x0a);
			USART_Transmit(0x0d);

			move_dir_fwd_rr = 'x';
		}
		else
			OCR1A = 1455;
		break;
	}
	case 's':
	{
		OCR1A = 1600;
		break;
	}
	case 'x':
	{
		OCR1A = 1500;
		case_f = 0;
		break;
	}
	case 'f':
	{
		if(!case_f){
			USART_Transmit('M');
			USART_Transmit('o');
			USART_Transmit('d');
			USART_Transmit('o');
			USART_Transmit(' ');
			USART_Transmit('A');
			USART_Transmit('u');
			USART_Transmit('t');
			USART_Transmit('o');
			USART_Transmit('n');
			USART_Transmit('o');
			USART_Transmit('m');
			USART_Transmit('o');
			USART_Transmit(0x0a);
			USART_Transmit(0x0d);
			case_f = 1;
		}
		if((50 < total_timer2_inc) && (total_timer2_inc <= 90)){
			OCR1A = 1500;
			rr_ok = 1;
		}
		else if((total_timer2_inc < 50) && (rr_ok==0)){
			OCR1A = 1600;
			_delay_ms(80);
			OCR1A = 1500;
			_delay_ms(40);

			rr_ok = 1;
		}
		else if((total_timer2_inc < 50) && (rr_ok==1)){
			OCR1A = 1600;
		}
		else if(total_timer2_inc > 90)
			OCR1A = 1455;
		rr_ok = 0;

		break;
	}
	default:{
		OCR1A = 1500;
		break;
	}
	}
}
/***************************************************************************************************/
int main (void)
{
	cli();

	DDRD |= (1<<PD4) | (1<<PD5) | (1<<PD6); // OC1B no servo e OC1A no esc.
	DDRD &= ~(1<<PD2); // INT0 - entrada do "ECHO"

	DDRB |= (1<<PB0); // Saída para o "trigger" do hc-05

	Timer1_Init();
	Timer2_Init();
	USART_Init(ubrr);
	INT_Settings();

	OCR1A = 1500;
	OCR1B = 1500;
	_delay_ms(44);

	sei();
	while(1)
	{
		_move_l_r(move_dir_l_r);
		_move_fwd_rr(move_dir_fwd_rr);
	}
}

ISR (TIMER2_OVF_vect)
{
	timer2_ovf++;
	time_to_send++;
	if(time_to_send >= 10)
	{
		//send_trigger();
		PORTB &= ~(1<<PB0);
		_delay_loop_1(0);
		PORTB |= (1<<PB0);
		_delay_loop_1(0);
		_delay_loop_1(0);
		_delay_loop_1(0);
		trigger_unlock = 1;
		PORTB &= ~(1<<PB0);
		time_to_send = 0;
	}
}

ISR (INT0_vect)
{
	if(trigger_unlock)
	{

		if(echo_ok)
		{
			total_timer2_inc = 256 * (timer2_ovf) + TCNT2;
			echo_ok = 0;
			trigger_unlock = 0;
		}
		else
		{
			timer2_ovf = 0;
			TCNT2 = 0;
			echo_ok = 1;
		}
	}
}

ISR (USART_RXC_vect)
{
	move_dir = UDR;
	move_dir_fwd_rr = ((move_dir == 'w') || (move_dir=='s') || (move_dir=='x') || (move_dir=='f'))?move_dir:move_dir_fwd_rr;
	move_dir_l_r = ((move_dir == 'a') || (move_dir=='d') || (move_dir=='c'))?move_dir:move_dir_l_r;
}
