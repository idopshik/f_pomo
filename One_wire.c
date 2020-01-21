/*
 * One_wire.h
 *
 * Created: 11.08.2014 19:47:59
 *  Author: idopshik
 */ 




#include <avr/io.h>

#define F_CPU 12000000UL // ���������� ������� ���������������� ��� �������� _delay_us

#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include <avr/interrupt.h>



extern void ReadedByteFromSlave(void);

char One_wire_buf[10]; // ���� ������ �� � �������� , �� ������ �� ����� - ����� ������ �� ����. (������ ���� - ��).
 
unsigned char do_1wire_or_nobody_home = 1;
unsigned char one_w_i = 0;


#define ONEWIRE_PORT PORTC
#define ONEWIRE_DDR DDRC
#define ONEWIRE_PIN PINC
#define ONEWIRE_PIN_NUM PC0

/*
const uint8_t PROGMEM onewire_crc_table[] = {
  0x00, 0x5e, 0xbc, 0xe2, 0x61, 0x3f, 0xdd, 0x83, 0xc2, 0x9c, 0x7e, 0x20, 0xa3, 0xfd, 0x1f, 0x41, 
  0x9d, 0xc3, 0x21, 0x7f, 0xfc, 0xa2, 0x40, 0x1e, 0x5f, 0x01, 0xe3, 0xbd, 0x3e, 0x60, 0x82, 0xdc, 
  0x23, 0x7d, 0x9f, 0xc1, 0x42, 0x1c, 0xfe, 0xa0, 0xe1, 0xbf, 0x5d, 0x03, 0x80, 0xde, 0x3c, 0x62, 
  0xbe, 0xe0, 0x02, 0x5c, 0xdf, 0x81, 0x63, 0x3d, 0x7c, 0x22, 0xc0, 0x9e, 0x1d, 0x43, 0xa1, 0xff, 
  0x46, 0x18, 0xfa, 0xa4, 0x27, 0x79, 0x9b, 0xc5, 0x84, 0xda, 0x38, 0x66, 0xe5, 0xbb, 0x59, 0x07, 
  0xdb, 0x85, 0x67, 0x39, 0xba, 0xe4, 0x06, 0x58, 0x19, 0x47, 0xa5, 0xfb, 0x78, 0x26, 0xc4, 0x9a, 
  0x65, 0x3b, 0xd9, 0x87, 0x04, 0x5a, 0xb8, 0xe6, 0xa7, 0xf9, 0x1b, 0x45, 0xc6, 0x98, 0x7a, 0x24, 
  0xf8, 0xa6, 0x44, 0x1a, 0x99, 0xc7, 0x25, 0x7b, 0x3a, 0x64, 0x86, 0xd8, 0x5b, 0x05, 0xe7, 0xb9, 
  0x8c, 0xd2, 0x30, 0x6e, 0xed, 0xb3, 0x51, 0x0f, 0x4e, 0x10, 0xf2, 0xac, 0x2f, 0x71, 0x93, 0xcd, 
  0x11, 0x4f, 0xad, 0xf3, 0x70, 0x2e, 0xcc, 0x92, 0xd3, 0x8d, 0x6f, 0x31, 0xb2, 0xec, 0x0e, 0x50, 
  0xaf, 0xf1, 0x13, 0x4d, 0xce, 0x90, 0x72, 0x2c, 0x6d, 0x33, 0xd1, 0x8f, 0x0c, 0x52, 0xb0, 0xee, 
  0x32, 0x6c, 0x8e, 0xd0, 0x53, 0x0d, 0xef, 0xb1, 0xf0, 0xae, 0x4c, 0x12, 0x91, 0xcf, 0x2d, 0x73, 
  0xca, 0x94, 0x76, 0x28, 0xab, 0xf5, 0x17, 0x49, 0x08, 0x56, 0xb4, 0xea, 0x69, 0x37, 0xd5, 0x8b, 
  0x57, 0x09, 0xeb, 0xb5, 0x36, 0x68, 0x8a, 0xd4, 0x95, 0xcb, 0x29, 0x77, 0xf4, 0xaa, 0x48, 0x16, 
  0xe9, 0xb7, 0x55, 0x0b, 0x88, 0xd6, 0x34, 0x6a, 0x2b, 0x75, 0x97, 0xc9, 0x4a, 0x14, 0xf6, 0xa8, 
  0x74, 0x2a, 0xc8, 0x96, 0x15, 0x4b, 0xa9, 0xf7, 0xb6, 0xe8, 0x0a, 0x54, 0xd7, 0x89, 0x6b, 0x35  
};
*/

// ������������� ������ ������� �� ���� 1-wire
inline void onewire_low() {
  ONEWIRE_DDR |= _BV(ONEWIRE_PIN_NUM);
}

// "���������" ���� 1-wire
inline void onewire_high() {
  ONEWIRE_DDR &= ~_BV(ONEWIRE_PIN_NUM);
}

// ������ �������� ������ �� ���� 1-wire
inline uint8_t onewire_level() {
  return ONEWIRE_PIN & _BV(ONEWIRE_PIN_NUM);
}


// ����������� � ������� ���� ����� ������ ���� ��������� "�������" ������������� ��������
#define ONEWIRE_STRONG_DDR DDRB
#define ONEWIRE_STRONG_PORT PORTB
#define ONEWIRE_STRONG_PIN_NUM PB1

// ��������� "�������" ��������
void onewire_strong_enable() {
  // ��� ���������� ������� ������ �� ����, ������� ���������� ������� ��������
  ONEWIRE_STRONG_PORT |= _BV(ONEWIRE_STRONG_PIN_NUM);
  // ����� - ������� �����������
  ONEWIRE_STRONG_DDR |= _BV(ONEWIRE_STRONG_PIN_NUM); 
}

// ���������� "�������" ��������
void onewire_strong_disable() {
  // ��� ���������� ������� ������ �� ����, ������� ���������� ������� �����������
  ONEWIRE_STRONG_DDR &= ~_BV(ONEWIRE_STRONG_PIN_NUM); 
  // ����� - ������� ��������
  ONEWIRE_STRONG_PORT &= ~_BV(ONEWIRE_STRONG_PIN_NUM);
}


// ����� ������� ������ (reset), ������� �������� ������� �����������.
// ���� ������� ����������� �������, ���������� ��� ���������� � ���������� 1, ����� ���������� 0 
uint8_t onewire_reset() 
{
  onewire_low();
  _delay_us(640); // ����� 480..960 ���
  onewire_high();
  _delay_us(2); // ����� ����������� �������������� ���������, ����� ������� ������� ������� �� ����
  // ��� �� ����� 60 �� �� ��������� �������� �����������;
  for (uint8_t c = 80; c; c--) {
    if (!onewire_level()) {
      // ���� ��������� ������� �����������, ��� ��� ���������
      while (!onewire_level()) { } // ��� ����� ������� �����������
      return 1;
    }
    _delay_us(1);
  }
  return 0;
}


// ���������� ���� ���
// bit ������������ ��������, 0 - ����, ����� ������ �������� - �������
void onewire_send_bit(uint8_t bit) {
  onewire_low();
  if (bit) {
    _delay_us(5); // ������ �������, �� 1 �� 15 ��� (� ������ ������� �������������� ������)
    onewire_high();
    _delay_us(90); // �������� �� ���������� ��������� (�� ����� 60 ���)
  } else {
    _delay_us(90); // ������� ������� �� ���� �������� (�� ����� 60 ���, �� ����� 120 ���)
    onewire_high();
    _delay_us(5); // ����� �������������� �������� ������� �� ���� + 1 �� (�������)
  }
}

// ���������� ���� ����, ������ ������ ���, ������� ��� �����
// b - ������������ ��������
void onewire_send(uint8_t b) {
  for (uint8_t p = 8; p; p--) {
    onewire_send_bit(b & 1);
    b >>= 1;
  }
}


// ������ �������� ����, ������������ ������������.
// ���������� 0 - ���� ������� 0, �������� �� ���� �������� - ���� �������� �������
uint8_t onewire_read_bit() {
  onewire_low();
  _delay_us(2); // ������������ ������� ������, ������� 1 ���
  onewire_high();
  _delay_us(8); // ����� �� ������� �������������, ����� �� ����� 15 ���
  uint8_t r = onewire_level();
  _delay_us(80); // �������� �� ���������� ����-�����, ������� 60 ��� � ������ ������� ������
  return r;
}

// ������ ���� ����, ���������� �����������, ������� ��� �����, ���������� ����������� ��������
uint8_t onewire_read() {
  uint8_t r = 0;
  for (uint8_t p = 8; p; p--) {
    r >>= 1;
    if (onewire_read_bit())
      r |= 0x80;
  }
  return r;
}


// ��������� �������� ���������� ����� crc ����������� ���� ��� ����� b.
// ���������� ���������� �������� ����������� �����
uint8_t onewire_crc_update(uint8_t crc, uint8_t b) 
{
  //return pgm_read_byte(&onewire_crc_table[crc ^ b]);
  
       for (uint8_t p = 8; p; p--) {
        crc = ((crc ^ b) & 1) ? (crc >> 1) ^ 0b10001100 : (crc >> 1);
         b >>= 1;
         }
         return crc;
}


// ��������� ������������������ ������������� (reset + ������� ������� �����������)
// ���� ������� ����������� �������, ��������� ������� SKIP ROM
// ���������� 1, ���� ������� ����������� �������, 0 - ���� ���
uint8_t onewire_skip() {
  if (!onewire_reset())
    return 0;
  onewire_send(0xCC);
  return 1;
}


// ��������� ������������������ ������������� (reset + ������� ������� �����������)
// ���� ������� ����������� �������, ��������� ������� READ ROM, ����� ������ 8-�������� ��� ����������
//    � ��������� ��� � ����� �� ��������� buf, ������� � �������� �����
// ���������� 1, ���� ��� ��������, 0 - ���� ������� ����������� �� �������
uint8_t onewire_read_rom(uint8_t * buf) {
  if (!onewire_reset())
    return 0; 
  onewire_send(0x33);
  for (uint8_t p = 0; p < 8; p++) {
    *(buf++) = onewire_read();
  }
  return 1;
}


// ��������� ������������������ ������������� (reset + ������� ������� �����������)
// ���� ������� ����������� �������, ��������� ������� MATCH ROM, � ���������� 8-�������� ��� 
//   �� ��������� data (������� ���� �����)
// ���������� 1, ���� ������� ����������� �������, 0 - ���� ���
uint8_t onewire_match(uint8_t * data) {
  if (!onewire_reset())
    return 0;
  onewire_send(0x55);
  for (uint8_t p = 0; p < 8; p++) {
    onewire_send(*(data++));
  }
  return 1;
}


// ���������� ��� �������� �������������� ���������� ������
uint8_t onewire_enum[8]; // ��������� �������������� ����� 
uint8_t onewire_enum_fork_bit; // ��������� ������� ���, ��� ���� ��������������� (������� � �������)

// �������������� ��������� ������ ������� ���������
void onewire_enum_init() {
  for (uint8_t p = 0; p < 8; p++) {
    onewire_enum[p] = 0;
  }      
  onewire_enum_fork_bit = 65; // ������ �������
}

// ����������� ���������� �� ���� 1-wire � �������� ��������� �����.
// ���������� ��������� �� �����, ���������� �������������� �������� ������, ���� NULL, ���� ����� �������
uint8_t * onewire_enum_next() {
  if (!onewire_enum_fork_bit) { // ���� �� ���������� ���� ��� �� ���� �����������
    return 0; // �� ������ ������� ������ �� ���������
  }
  if (!onewire_reset()) {
    return 0;
  }  
  uint8_t bp = 8;
  uint8_t * pprev = &onewire_enum[0];
  uint8_t prev = *pprev;
  uint8_t next = 0;
  
  uint8_t p = 1;
  onewire_send(0xF0);
  uint8_t newfork = 0;
  for(;;) {
    uint8_t not0 = onewire_read_bit();
    uint8_t not1 = onewire_read_bit();
    if (!not0) { // ���� ������������ � ������� ��� ����
      if (!not1) { // �� ����� ������������� ��� 1 (�����)
        if (p < onewire_enum_fork_bit) { // ���� �� ����� �������� ������� ������������ ����, 
          if (prev & 1) {
            next |= 0x80; // �� �������� �������� ���� �� �������� �������
          } else {
            newfork = p; // ���� ����, �� �������� ����������� �����
          }          
        } else if (p == onewire_enum_fork_bit) {
          next |= 0x80; // ���� �� ���� ����� � ������� ��� ��� ������ �������� � ����, ������� 1
        } else {
          newfork = p; // ������ - ������� ���� � ���������� ����������� �����
        }        
      } // � ��������� ������ ���, ������� ���� � ������
    } else {
      if (!not1) { // ������������ �������
        next |= 0x80;
      } else { // ��� �� ����� �� ������ - ��������� ��������
        return 0;
      }
    }
    onewire_send_bit(next & 0x80);
    bp--;
    if (!bp) {
      *pprev = next;
      if (p >= 64)
        break;
      next = 0;
      pprev++;
      prev = *pprev;
      bp = 8;
    } else {
      if (p >= 64)
        break;
      prev >>= 1;
      next >>= 1;
    }
    p++;
  }
  onewire_enum_fork_bit = newfork;
  return &onewire_enum[0];
}

// ��������� ������������� (�����) �, ���� ������� ����������� �������,
// ��������� MATCH_ROM ��� ���������� ���������� ������� onewire_enum_next ������
// ���������� 0, ���� ������� ����������� �� ��� �������, 1 - � ���� ������
uint8_t onewire_match_last() {
  return onewire_match(&onewire_enum[0]);
}


// �������� �� UART ���� �����, ���������� ����������� ������� ������ dig �� sub.
// ���������� ������� ����� �������
uint16_t uart_digit(uint16_t dig, uint16_t sub) {
  char c = '0';
  while (dig >= sub) {
    dig -= sub;
    c++;
  }
  
   One_wire_buf[one_w_i++]= c;
  return dig;
}



// �������� � UART ���������� ������������� ����� � ������������� ������, 
// ��� ������� ����� ������������ �������� 4 ���������
void uart_num(int16_t num) {
  uint16_t unum; // ����� ��� �����

    unum = num;
 
  uint16_t snum =  unum >> 4; // ����������� ������� �����
  if (snum >= 10) {
    if (snum >= 100) {
      if (snum >= 1000) {
		 
        snum = uart_digit(snum, 1000); // 4� ������
      }
      snum = uart_digit(snum, 100); // 3� ������
    }
    snum = uart_digit(snum, 10); // 2� ������
  }
  uart_digit(snum, 1); // 1� ������
  
 One_wire_buf[one_w_i++]= ','; // ���������� �����������
  
  uart_digit((((uint8_t)(unum & 0x0F)) * 10) >> 4, 1); // ������� �����
 One_wire_buf[one_w_i++]= '*';
 One_wire_buf[one_w_i++]= 'C';
}


void send_skip_rom(void)
{
   if (onewire_skip())
		{onewire_send(0x44); // ...����������� ����� ����������� �� ���� �������������
			do_1wire_or_nobody_home =1;
		}
	else {
One_wire_buf[0]='?';
		One_wire_buf[1]='?';
		One_wire_buf[2]='?';
		
	}	
}

///������� �� ������, �������� �� ��������������, ��������� � ������������� ���������� ����� ����������� ������
////////////////////////////////////////////////////////////////////////////////////////////////////////////




/*
//////////////////// ������ � UART ////////////////////

// ���������� ���� ���� � UART
void uart_write(uint8_t data) {
  while (!(UCSRA & (1 << UDRE))); // �������� ������������ ������ ��������
  UDR = data; // ���������� ���� � �����, �������� ���������� �������������.
}

// �������� �� UART ���� �����, ���������� ����������� ������� ������ dig �� sub.
// ���������� ������� ����� �������
uint16_t uart_digit(uint16_t dig, uint16_t sub) {
  char c = '0';
  while (dig >= sub) {
    dig -= sub;
    c++;
  }
  uart_write(c);
  return dig;
}

// �������� � UART ���������� ������������� ����� � ������������� ������, 
// ��� ������� ����� ������������ �������� 4 ���������
void uart_num(int16_t num) {
  uint16_t unum; // ����� ��� �����
  if (num < 0) { // ������������� �����. �������� ����
    unum = -num;
    uart_write('-'); 
  } else {
    unum = num;
  }
  uint16_t snum =  unum >> 4; // ����������� ������� �����
  if (snum >= 10) {
    if (snum >= 100) {
      if (snum >= 1000) {
        snum = uart_digit(snum, 1000); // 4� ������
      }
      snum = uart_digit(snum, 100); // 3� ������
    }
    snum = uart_digit(snum, 10); // 2� ������
  }
  uart_digit(snum, 1); // 1� ������
  uart_write('.'); // ���������� �����������
  uart_digit((((uint8_t)(unum & 0x0F)) * 10) >> 4, 1); // ������� �����
}

// �������� � UART ����������������� ���������� ������������� �����
void uart_hex(uint8_t hexdig) {
  uart_write((hexdig >> 4) + (((hexdig >> 4) >= 10) ? ('A' - 10) : '0'));
  uart_write((hexdig & 0x0F) + (((hexdig & 0x0F) >= 10) ? ('A' - 10) : '0'));
}

// �������� � UART ������� �������� ������
void uart_newline() {
  uart_write('\r');
  uart_write('\n');
}

*/
//int main(void)
//{
  
  
 // onewire_strong_disable();
  
 /* // ������������� UART
  UCSRA &= ~(1 << U2X); // ��������� �������� ��������
  UCSRB = (1 << TXEN); // �������� ���������� USART
  UCSRC = 0b1000110; // ����������� �����, �������� �������� - ��������, 1 ���� ���, 8 ��� ������
  UBRRH = 0;
  UBRRL = 51; // 9600 ���
  
  while(1)
  {
    if (onewire_skip()) { // ���� � ��� �� ���� ���-�� ������������,...
      onewire_send(0x44); // ...����������� ����� ����������� �� ���� �������������
      onewire_strong_enable(); // ���������� ��������� ��������
      _delay_ms(900); // ������� �� 750 ��.
      onewire_strong_disable(); // ����������� ��������� �������� ��� ����������� ������
      
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
          uart_hex(d);
          uart_write(' ');
          crc = onewire_crc_update(crc, d);
          d = *(p++);
        }
        if (crc) {
          // � ����� ������ ���������� ����. ���� �� ���, ����� ��������� �� ������
          uart_write('C');
          uart_write('R');
          uart_write('C');
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
              uart_write('~');
              uart_write('C');
              uart_write('R');
              uart_write('C');
              uart_hex(crc);
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
            uart_write('?');
          }
        }
        uart_newline();
      }      
      uart_write('.');
    } else {
      uart_write('-');
    }
    uart_newline();
    _delay_ms(2000); // ��������� ��������
  }
}

*/














				 


    

 



	                  
					 
					 


