/*
 * HW_definitions.h
 *
 * Created: 05.06.2016 10:30:05
 *  Author: isairton
 */ 


#ifndef HW_DEFINITIONS_H_
#define HW_DEFINITIONS_H_


#include <avr/io.h>

 /*
 ///////////////////////////// ����� ����������� �����������////////////////////////////////////////////////

 ������ �� LIS
 PD3 - ���� ���������� �� LIS
 PB1 - ����� ��� ������. PB2 - �� ���������
 PC1 - ����� �� ����, ����� ����������� ������� � PC0. ����� � ��� ���� �������
 PC2 - ������������.
 PC7 - �����������
 PC3 - ���� npn ��� �������
 ����������
 pd2 -������� LED
 pd4 - ������ LED. ��� ������ � �����
 ����� � 595
 PB6 DS
 PB7 EN
 PD5	Latch
 PD6	Schift
 ������, �� �����, ��� � ��������������
 PD7- �������
 PB0 - ������
 */
  #define uart_new_line_Macro {uart_putc(0x0A); uart_putc(0x0D);}

 #define Led_Yellow_ON PORTD|=(1<<PD2);
 #define Led_Yellow_OFF PORTD&=~(1<<PD2);
 #define Led_RED_ON PORTD|=(1<<PD4);
 #define Led_RED_OFF PORTD&=~(1<<PD4);
 #define Buzzer_ON PORTC|=(1<<PC3);
 #define Buzzer_OFF PORTC&=~(1<<PC3);
 
 

void Hardware_init (void)
{
	
	// ������ ��� TWI
	PORTC |= (1<<4)|(1<<5);						 // ������� �������� �� ���� TWI
	DDRC &=~(1<<4|1<<5);
	
	//DDRC &=~((1<<2)|(1<<7));					 // ����� ���   //�����������
	PORTC &=~((1<<2)|(1<<7));					 
	DDRC |= (1<<7);
	
	// ������
	DDRB &=~(1<<0);								//������1
	DDRD &=~(1<<7);								//������2
	PORTB |= (1<<0);
	PORTD |= (1<<7);
	
	DDRC |= (1<<3);								 // ������

	DDRD|= (1<<2)|(1<<4);						 // ����������
	
	DDRC |= (1<<3);								 // ������

}


#endif /* HW_DEFINITIONS_H_ */
