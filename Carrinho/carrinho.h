/*
 * carrinho.h
 * Autor: Pedro Vinicius
 * Última modificação: 07/03/2016	22:17
 * 
 * ######### Funções do projeto #########
 *
 *
 */

#ifndef CARRINHO_H_INCLUDED
#define CARRINHO_H_INCLUDED

/*	Não estou usando mais.
#define forward		1
#define backward	2
#define fbreak		3
#define	left		0
#define right		1
*/
#define BAUD 9600
#define ubrr F_CPU/16/BAUD - 1

//void ADC_Init(void);

unsigned char USART_Receive( void );

void USART_Init(uint16_t my_ubrr);
void USART_Transmit( unsigned char __data);

void Timer2_Init(void);
void Timer1_Init(void);
void INT_Settings(void);

#endif // CARRINHO_H_INCLUDED
