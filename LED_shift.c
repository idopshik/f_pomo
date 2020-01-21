
#include "LED_shift.h"

 #include <avr/pgmspace.h>
 // ������ PORTB ������ ���� ShiftPort


//SH_CP   ---- CLOCK
//ST_CP  ---- LATCH
//DS  ----  Data

 /*
 
 0 - 0x20BC
 1 - 0x2080
 2 - 0x2234
 3 - 0x22A4
 4 - 0x2288;  //  0010 0010 1000 1000
 5 - 0x2AC;  //  0000 0010 1010 1100
 6 - 0x2BC;  //  0000 0010 1011 1100
 7 - 0x2084;  //  0010 0000 1000 0100
 8 - 0x22BC;  //  0010 0010 1011 1100
 9 - 0x22AC;
 10 -NO_
 11  E - 0x23C;  //  0000 0010 0011 1100
 12  C - 0x3C;  //  0011 1100
 13  � - 0x1C;  //  0001 1100
 14  H - 0x2298;  //  0010 0010 1001 1000




 */
 
 
 // �������� ���� ��� ����������� ����� ����������� ���� 595 � ��������� ����� LED ����������� ��������
// ����� ���������� ����. ��������� ������� �� �������� � ����������, ��� ������ ���� �� ������ ���� 595 ��� ������ �����.

const unsigned int PROGMEM NumCodes[] =
{
	0x20BC,0x2080,0x2234,0x22A4,0x2288,0x2AC,0x2BC,0x2084,0x22BC,0x22AC   , 0x0000,0x23C, 0x3C,0x1C,0x2298// ������� ���� ������ - ��������!
};


void PutOneDigit(unsigned char Num,unsigned char Digit,unsigned char DOT)
{
	uint16_t localvar;
	
	switch(Digit)
	{
		case 0:localvar = 0x1802;
		break;
		case 1:localvar = 0xC02;
		break;
		case 2:localvar = 0x1402;
		break;
		case 3:localvar = 0x1C00;
		break;
		default: localvar = 0;
	}
	
	if(DOT)localvar|=0x40; // ������ �����

 shift( pgm_read_word(NumCodes+Num)|localvar) ;
}

void shift( unsigned int data){					// ��������� ������� �����.
	uint8_t localvar;
	uint8_t j = 2;								//����������� ����� ��� ����������, ��� ��������� ���� ������!
	
	ShiftPort &= ~(1 << ST_CP); 				// Set the register-clock pin low

	localvar = (data>>8);						// ������ ������� ����, ������� �������, ��� ���� "�������" �� ������ 595
	do
	{
		for (signed char i = 7; i >= 0; i--){	// Now we are entering the loop to shift out 8+ bits

			ShiftPort &= ~(1 << SH_CP); 			// Set the serial-clock pin low

			PORTB |= (((localvar&(0x01<<i))>>i) << DS ); 	// Go through each bit of data and output it

			ShiftPort |= (1 << SH_CP); 				// Set the serial-clock pin high

			PORTB &= ~(1 << DS );				// Set the datapin low again 
		}
		localvar = data;							// ��� ������ ��� ���� �������, � ������ ��� ���� ��������!
		j--;											// ������ �������� 1, ������ - 0, � ������� ������� �� �����.
		
	} while (j);
	
	ShiftPort |= (1 << ST_CP);					// Set the register-clock pin high to update the output of the shift-register
	}
	
	
	
	
	
	void Hardware_set_for_shift (void)
	{
		ShiftDDR |=(1<<ST_CP)|(1<<SH_CP);
		ShiftPort &=~((1<<ST_CP)|(1<<SH_CP));
		
		DDRB |= (1<<6)|(1<<7); //DS � OE
		PORTB  &=~(1<<DS);
		PORTB  &=~(1<<7);// ����� OE ������ ������ ���� �� "�����". ������ �������.
	}

