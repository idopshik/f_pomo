/*
 * One_wire.h
 *
 * Created: 21.02.2015 22:32:40
 *  Author: idopshik
 */ 


#ifndef ONE_WIRE_H_
#define ONE_WIRE_H_


//inline
// void onewire_low();
//inline 
//void onewire_high();

//inline
 uint8_t onewire_level();
void onewire_strong_enable();
void onewire_strong_disable();
uint8_t onewire_reset();
void onewire_send_bit(uint8_t bit);
void onewire_send(uint8_t b);
uint8_t onewire_read_bit();
uint8_t onewire_read();
uint8_t onewire_crc_update(uint8_t crc, uint8_t b);
uint8_t onewire_skip();
uint8_t onewire_read_rom(uint8_t * buf);
uint8_t onewire_match(uint8_t * data);
void onewire_enum_init();
uint8_t * onewire_enum_next();
uint16_t uart_digit(uint16_t dig, uint16_t sub);
void uart_num(int16_t num);
void ONE_WIRE_DO_IT(void);
void send_skip_rom(void);	



#endif /* ONE_WIRE_H_ */

/*
//������� �� ������, �������� �� ��������������, ��������� � ������������� ���������� ����� ����������� ������
////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ONE_WIRE_DO_IT(void)
 
  {
cli(); /// �������� ����������, ����� �������� ������� ���������
	PORTC^=(1<<3); ///����� ������ ���
     PORTA^=(1<<6);
Write_MCP23008(7,' '); ////��������� �� �������� ������ �������.

    if (do_1wire_or_nobody_home) { // ���� � ��� �� ���� ���-�� ������������,...
      
     // onewire_strong_enable(); // ���������� ��������� ��������
      //_delay_ms(900); // ������� �� 750 ��.
     // onewire_strong_disable(); // ����������� ��������� �������� ��� ����������� ������
      
      onewire_enum_init(); // ������ ������������ ���������
      for(;;) {
        uint8_t * p = onewire_enum_next(); // ��������� �����
        if (!p) 
          break;
        // ����� ����������������� ������ ������ � UART � ������� CRC
        uint8_t d = *(p++);
        uint8_t crc = 0;
        uint8_t family_code = d; // ���������� ������� ����� (��� ���������)
        for (uint8_t i = 0; i < 8; i++) {
      //    uart_hex(d);
       //   uart_write(' ');
          crc = onewire_crc_update(crc, d);
          d = *(p++);
        }
        if (crc) {
          // � ����� ������ ���������� ����. ���� �� ���, ����� ��������� �� ������
          Write_MCP23008(1,'c');
           Write_MCP23008(1,'r');
           Write_MCP23008(1,'c');
        } else {
          if ((family_code == 0x28) || (family_code == 0x22) || (family_code == 0x10)) { 
            // ���� ��� ��������� ������������� ������ �� ���������... 
            // 0x10 - DS18S20, 0x28 - DS18B20, 0x22 - DS1822
            // ������� ������ scratchpad'�, ������ �� ���� crc
            onewire_send(0xBE); 
            uint8_t scratchpad[8];
            crc = 0;
            for (uint8_t i = 0; i < 8; i++) {
              uint8_t b = onewire_read();
              scratchpad[i] = b;
              crc = onewire_crc_update(crc, b);
            }
            if (onewire_read() != crc) {
              // ���� ����������� ����� ���������� �� �������.
               Write_MCP23008(1,'$');
              Write_MCP23008(1,'c');
           Write_MCP23008(1,'r');
           Write_MCP23008(1,'c');
          //    uart_hex(crc);
            } else {
              int16_t t = (scratchpad[1] << 8) | scratchpad[0];
              if (family_code == 0x10) { // 0x10 - DS18S20 
                // � DS18S20 �������� ����������� ������ 1 ������ � ������� �����.
                // �������� �������� ��������� ����� ������ ���� 6 (COUNT_REMAIN) �� scratchpad
                t <<= 3;
                if (scratchpad[7] == 0x10) { // �������� �� ������ ������
                  t &= 0xFFF0;
                  t += 12 - scratchpad[6];
                }
              } // ��� DS18B20 DS1822 �������� �� ��������� 4 ���� � ������� �����
              // ����� �����������
              uart_num(t);
            }            
          } else {
            // ����������� ����������
            Write_MCP23008(1,'?');
          }
        }
        //uart_newline();
      }      
     // uart_write('.');
    }
	do_1wire_or_nobody_home=0; //// ������ ����� �� ��� ����, ����� ��� ��� ��������� ������� ����������.
	sei(); /// �������� ����������, ����� �� �������� ���������.
	
			}

*/