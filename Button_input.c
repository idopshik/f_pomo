/*
 * Button_input.c
 *
 * Created: 05.06.2016 10:09:07
 *  Author: isairton
 ����� �����������, ��������� � ������� ��� :)
 */ 
#include "Button_input.h"
#include <avr/io.h>


// ��� �������� � ���������� ���������� �������
void Within_ISR_button_service (void)
{
	
	//���� �����, �� �� ������� ��� �� ����� ����� ����� ���� ���������� �� ���. ��������, �������� �� ������ �������.
	Button_Timer_Counter++;
	// ���� ������������ � ��� ���������� ���� �� ���� �� ��������
	if((Button_Timer_Counter>ButtonTimerOverFlow)&&(Button_Timer_Flag&ButtonTimerSet_0))    Button_Timer_Flag|=ButtonLongReady_0;
	if((Button_Timer_Counter>ButtonTimerOverFlow)&&(Button_Timer_Flag&ButtonTimerSet_1))    Button_Timer_Flag|=ButtonLongReady_1;
	
}


//******************************************����������***********************************************************


unsigned int integrator[NumberOfButtons];
unsigned char output[NumberOfButtons] = {1,1}; // �� ���� ��� �� �������(�� ����� ��� ����� ��� �����������). ����� ��� ������� �������������� �������. 

void SetButtonTimer(uint8_t T)
{
	Button_Timer_Counter = 0;				// �������� � ����
	switch(T)
	{
		case 0:
		Button_Timer_Flag |=ButtonTimerSet_0;		// ������ ������ ����
		break;
		case 1:
		Button_Timer_Flag |=ButtonTimerSet_1;
		break;
		case 11:
		Button_Timer_Flag |=ButtonTimerSet_0|ButtonTimerSet_1; //���
		break;
		
	}
	
}



char f_integrator (unsigned char input, unsigned char button_num)
{
	

	if (input == 0)
	{
		if (integrator[button_num] > 0)integrator[button_num]--;
	}
	else if (integrator[button_num] < MAXIMUM) integrator[button_num]++;
	
	
	if (integrator[button_num] == 0) output[button_num] = 0;
	
	else if (integrator[button_num] >= MAXIMUM)
	{
		output[button_num] = 1;
		integrator[button_num] = MAXIMUM;  // defensive code if integrator got corrupted
	}
	return output[button_num];
}




// ���������� ���� ������
//����� �������- ������1. ����� - ������2. ������� ������1. ������� ������2
uint8_t ButtonCheck (void)
{
	uint8_t var0,var1,Button_output;
	Button_output = 0;			// ������ ��� � ����
	//�����������
	
	var0 = f_integrator(PINB &(1<<0),0);			// ������ ��������� ��� bounce
	var1 = f_integrator(PIND &(1<<7),1);
	
	
	// ���� ������ ������
	 if(!var0)
	{
		// �������������� �� ��������� ��������
		if(!(Button_Timer_Flag&ButtonRepeatPrevention_0))
		{
			//��������� ��������� ������� � ����� �������.
			if (!(Button_Timer_Flag&ButtonTimerSet_0))
			{
				//������������� ������,��� � ����� � ��� ���������
				SetButtonTimer(0); 
				Button_state |= ButtonPressed_0_MASK; // ������ �� �������� �������
			}
			//����� �� ������ �������� �������
			if(Button_Timer_Flag&ButtonLongReady_0)
			{
				
				// � ���� �� � ��� ������� �� ��� ��������, ������ ��� ������� ������?
				if (!(Button_state&ButtonPressed_SHORT_Double_MASK))
				{
					//�������� �� ����������� �������
					Button_output|=ButtonPressed_0_LONG_MASK;
				}
				//�����, ������ ��������������
				Button_Timer_Flag |= ButtonRepeatPrevention_0;
				// ������ ������ �� �������� �������
				Button_state &= ~ButtonPressed_0_MASK;
			}
			
		}
	}

	// ���� ������ ��������
	if(var0)
	{
		//��������� ���������, ���� �� �������� �������
		if (Button_state&ButtonPressed_0_MASK)
		{
			
			//����� ���� � ��� ����� ������ � �� ������ LONG ������ ������, � ��� �� ���� ��� ����������� SHORT_Double
			 if ((Button_state&ButtonPressed_1_MASK)&&(!(Button_state&ButtonPressed_SHORT_Double_MASK)))
			{
				Button_state|= ButtonPressed_SHORT_Double_MASK;		//������ ���� �������� ������� (��� ������ ������ � ������
				// ���� �������� ������� �������
				Button_output|=ButtonPressed_SHORT_Double_MASK;
				// ������ ������ ������ �� ������ ������ ���� ����.
				// ������ ��������� ������� ������ ������
			
			}
			//��� �� ������, � � ��� ��� ����� ������������� 
			 
			else if(!(Button_state&ButtonPressed_SHORT_Double_MASK))
			{
				//����, �������� ������� ��������, 
				Button_output|=ButtonPressed_0_MASK;
				
			}
			// ������ �� ���� ����
			else 
			{
				Button_state=0;// ������� ��� ������� ��� ���������� �������, ������
			}
			// ������� ��� ������� ��� ���������� ������� ��� ��� �����
				Button_state&= ~ButtonPressed_0_MASK;
		
		
		}
		// �������������� ������� ��������� ����� ���� ������
		Button_Timer_Flag &= ~ (ButtonLongReady_0 | ButtonTimerSet_0 | ButtonRepeatPrevention_0); // �� ����� ������ �������! ��, ������ ������� �� ������ ��������. �� ���� ������ ��� ��������� ����� �������� ����, ���� �� ����� ������ ����
		
	}





	// ���� ������ ������
	if(!var1)
	{
		// �������������� �� ��������� ��������
		if(!(Button_Timer_Flag&ButtonRepeatPrevention_1))
		{
			//����� �� ������ �������� �������
			if(Button_Timer_Flag&ButtonLongReady_1)
			{
				//�����, ������ ��������������
				Button_Timer_Flag |= ButtonRepeatPrevention_1;
				// ������ ������ �� �������� �������
				Button_state &= ~ButtonPressed_1_MASK;
				// � ���� �� � ��� ������� �� ��� ��������, ������ ��� ������� ������?
				if (!(Button_state&ButtonPressed_SHORT_Double_MASK))
				{
					//�������� �� ����������� �������
				Button_output|=ButtonPressed_1_LONG_MASK;
				}
				
			}
			// ����� ���� �� ���� �� ����������?
			if (!(Button_Timer_Flag&ButtonTimerSet_1))
			{
				//������������� ������
				SetButtonTimer(1);
				Button_state |= ButtonPressed_1_MASK; // ������ �� �������� �������
			}
		}
	}

	// ���� ������ ��������
	if(var1)
	{
		//��������� ���������, ���� �� �������� �������
		if (Button_state&ButtonPressed_1_MASK)
		{
		
			//����� ���� � ��� ����� ������ � �� ������ LONG ������ ������, � ��� �� ���� ��� ����������� SHORT_Double
			if ((Button_state&ButtonPressed_0_MASK)&&(!(Button_state&ButtonPressed_SHORT_Double_MASK)))
			{
				Button_state|= ButtonPressed_SHORT_Double_MASK;		//������ ���� �������� ������� � ������
				// ���� �������� ������� �������
				Button_output|=ButtonPressed_SHORT_Double_MASK;
				// ������ ������ ������ �� ������ ������ ���� ����.
				// ������ ��������� ������� ������ ������
				
			}
			//����� ���� � ��� ���� ������������� �� ������ ������?
			else if (!(Button_state&ButtonPressed_SHORT_Double_MASK))
			{
				//����, �������� ������� ��������,
				Button_output|=ButtonPressed_1_MASK;
				
			}
			// ����� ����������
			else
			{
				Button_state=0;// ������� ��� �������. ������
			}
			// ������� ��� ������� ��� ���������� �������
				Button_state&= ~ButtonPressed_1_MASK;	
		}
		// �������������� ������� ��������� ����� ���� ������ (������ ��� ������� ����� ���� � �������. ������� �� ������ ��������
		Button_Timer_Flag &= ~  (ButtonLongReady_1 | ButtonTimerSet_1 | ButtonRepeatPrevention_1);
	}

	return Button_output;
}


// ������ ��������� ��� bounce
 //var0 = f_integrator(PinButton &(1<<Button_1));
/// var1 = f_integrator_1(PinButton &(1<<Button_2));




//