
//#define F_CPU 80000000UL  // 8 MHz, внутренняя RC-цепочка
#include <avr/io.h>

#include <avr/interrupt.h>
#include <avr/eeprom.h>
#include <avr/pgmspace.h>
#include <util/delay.h>


#include <stddef.h>
#include <stdint.h>

#include "ds1307.h"
#include "LED_shift.h"
#include "HW_definitions.h"
#include "Button_input.h"

#include "avr/wdt.h"

//------------- Умолчания----------------//
#define BAUD 9600


#define ModeOFF	    0		//	Спим - читаем часы иногда.
#define ModeON   	1		//	Работаем




//---------------Переменные глобальные-------------------//






volatile uint16_t G_counter =0;
volatile uint8_t G_counter_char =0;


volatile unsigned char Digit = 0;
volatile unsigned char LED_string[4];

volatile unsigned int BeepDuration;
unsigned char const BeepPattern[] PROGMEM = {70,0,0};
//unsigned char const BeepPattern_LONG[] PROGMEM = {250,1,250,1,0,0};
unsigned char const BeepPattern_DoubleShort[] PROGMEM ={50,20,50,20,50,20,50,20,50,20,50,20,50,20,50,20,0,0};
unsigned char const BeepPattern_DoubleShort_Wrong[] PROGMEM = {100,30,100,30,100,30,100,30,0,0};
unsigned char const BeepPattern_Meloidic[] PROGMEM = {200,30,200,254,100,20,100,1,0,0};	
unsigned char const BeepPattern_SwitchOn[] PROGMEM = {80,250,40,250,40,200,40,150,40,150,40,100,40,100,40,40,40,40,20,20,15,20,10,0};
unsigned char const BeepPattern_SwitchOFF[] PROGMEM = {10,10,15,20,20,10,40,40,40,40,40,100,40,100,40,150,40,150,40,200,40,250,40,250,80, 0};		

unsigned char * BeeperPattern_pointer;     // Адресс массива
unsigned char BeeperFlag;
unsigned char BeeperPatternCounter;

volatile unsigned char Nested_counter =0;
volatile unsigned char Last_GlobalVar = 0;

volatile unsigned int G_time1 = 0;          // inside ISR counter

void (*f)(void) ;   // f - указатель

//---------------Прототипы-------------------//
void Get_time (void);

//--------------- Процедуры и функции-----------------------//


void Beeper_Activator(const uint8_t *pattern)
{
	BeeperPattern_pointer=(uint8_t *)pattern;
	BeeperFlag|= 0x80;
	BeeperPatternCounter = 0;
}

void Deal_with_Beeper(void)
{
	uint8_t var;
	if ( BeeperFlag & 0x80)
	{
		var = pgm_read_byte(BeeperPattern_pointer+BeeperPatternCounter);
		if (var == 0)
		{BeeperFlag = 0;
		BeeperPattern_pointer = 0;
		BeeperPatternCounter=0;
		Buzzer_OFF;
		return;
		}
		else if(BeeperFlag&0x01)
		{
			Buzzer_OFF;
			BeeperFlag&=0xFE;
		}
		else
		{
			Buzzer_ON;
			BeeperFlag|=0x01;
		}
		BeepDuration = var;
		BeeperPatternCounter++;
	}
}



ISR(TIMER0_OVF_vect)		// 2ms  // Обслуживаем индикацию
{
	G_counter_char++;
	
	Within_ISR_button_service();
	
	 Digit++;												//изменение разряда для мультиплексированного вывода.
	 if(Digit==4) Digit=0;
	 
	 if(10>1000) PutOneDigit(10,Digit,0);				// 190 ms чёрный экран - моргание. Не выполняется.
	 else
	 {
                                                     //Показываем время (точка посередине) или тем-ру
        if ((Digit==2)&&(G_counter_char>127)) PutOneDigit(LED_string[Digit],Digit,1);
        else PutOneDigit(LED_string[Digit],Digit,0);	  		 
	
    }
		//Попеременно светодиодами
        if(G_counter_char==0)
        {Led_Yellow_OFF;Led_RED_ON;}
        if(G_counter_char==127)
        {Led_RED_OFF;Led_Yellow_ON;}
}

ISR(TIMER2_OVF_vect)					//1ms // Логика датчика и задержек.
{
	
    G_time1++;
    if(G_time1>1000)					// Секунда
    {
	
		// Таймер для зуммера	
		if(BeepDuration>0)BeepDuration--;
		if(BeepDuration ==0)Deal_with_Beeper();

		G_counter++;

	}


	if (G_counter > 200) // каждые 200ms
	{
				Nested_counter++;
				if (Nested_counter >3)
				{
					f = Get_time;
					Nested_counter =0;
					Last_GlobalVar = 1;	
				}			
		G_counter =0;
	}	
}


inline void setupTimer_0 (void)
{			//переполнение каждые 2,048мс
	TCCR0 = 0;
	TCCR0 |= (1<<CS01)|(1<<CS00);	//64, 32 у этого таймера нет.
	TIMSK|=(1<<TOIE0);            // Enable Counter Overflow Interrupt
}

inline void SetupTimer_2 (void)
{			//переполнение каждые 1,024мс
	TCCR2 = 0;
	TCCR2 |= (1<<CS21)|(1<<CS20);	//32
	TIMSK|=(1<<TOIE2);            // Enable Counter Overflow Interrupt
}




void Get_time (void)
{	
    uint8_t var,hour,minute,second;

    ds1307_getdate(&var, &var, &var, &hour, &minute, &second);
    
    LED_string[3]=(hour % 100 / 10);
    LED_string[2]=(hour % 10);
    LED_string[1]=(minute % 100 / 10);
    LED_string[0]=(minute % 10);	
}


uint16_t DIG_digit(uint16_t dig, uint16_t sub,uint8_t DIGIT) {
	char c = 0;
	while (dig >= sub) {
		dig -= sub;
		c++;
	}
	LED_string[DIGIT]=c;				// Вожделенная строчка!
	return dig;
}



void DIG_num(int16_t num) {
	uint16_t unum; // число без знака

	unum = num;
	uint16_t snum =  unum >> 4; // отбрасывает дробную часть
	if (snum >= 10) {
		LED_string[3]=10;						 // Нафиг не нужен этот разряд в комнате.
		snum = DIG_digit(snum, 10,2); // 2й разряд
	}
	DIG_digit(snum, 1,1); // 1й разряд
	DIG_digit((((uint8_t)(unum & 0x0F)) * 10) >> 4, 1,0); // дробная часть
}





 int main (void)
 
 {
	wdt_enable(WDTO_500MS);
	 

	 	 //////////////////////////   H A R D W A R E   I N I T   /////////////////////////////////
Hardware_set_for_shift();
Hardware_init();
Buzzer_ON;  
sei();		  

	
_delay_ms(3);
setupTimer_0(); // LED и опрос кнопок
SetupTimer_2(); // Работа с акселерометром
ds1307_init();

f=NULL;

Buzzer_OFF;// зуммер выключился, загрузка окончена


	while(1)
	{
		wdt_reset(); // сбрасываем собачий таймер

		  switch(ButtonCheck())
		  {
			  case ButtonPressed_0_MASK:
			  Beeper_Activator(BeepPattern);
			  break;
			  case ButtonPressed_1_MASK:
			  Beeper_Activator(BeepPattern);
			  break;
			  case ButtonPressed_0_LONG_MASK:
			  Beeper_Activator(BeepPattern_Meloidic);
			  break;
			  case ButtonPressed_1_LONG_MASK:
			  Beeper_Activator(BeepPattern_Meloidic);
			  break;
			  case (ButtonPressed_0_LONG_MASK| ButtonPressed_1_LONG_MASK):
			  
			  break;
			  case ButtonPressed_SHORT_Double_MASK:
		
			  break;
		
		  }

        if(f)
        {
            cli();
            f();	// вызов функции если там есть чё.
            f=NULL;   // Предотвращаем цикличность.
            sei();
        }
	}
 }
