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
//команда на чтение, задержка на преобразование, получение и трансформация результата перед бесконечным циклом
////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ONE_WIRE_DO_IT(void)
 
  {
cli(); /// запретим прерывания, чтобы одновайр работал нормально
	PORTC^=(1<<3); ///тоггл третий бит
     PORTA^=(1<<6);
Write_MCP23008(7,' '); ////переходим на середину нижней строчки.

    if (do_1wire_or_nobody_home) { // Если у нас на шине кто-то присутствует,...
      
     // onewire_strong_enable(); // Включается усиленная подтяжка
      //_delay_ms(900); // Минимум на 750 мс.
     // onewire_strong_disable(); // Обязательно выключить подтяжку для дальнейшего обмена
      
      onewire_enum_init(); // Начало перечисления устройств
      for(;;) {
        uint8_t * p = onewire_enum_next(); // Очередной адрес
        if (!p) 
          break;
        // Вывод шестнадцатиричной записи адреса в UART и рассчёт CRC
        uint8_t d = *(p++);
        uint8_t crc = 0;
        uint8_t family_code = d; // Сохранение первого байта (код семейства)
        for (uint8_t i = 0; i < 8; i++) {
      //    uart_hex(d);
       //   uart_write(' ');
          crc = onewire_crc_update(crc, d);
          d = *(p++);
        }
        if (crc) {
          // в итоге должен получиться ноль. Если не так, вывод сообщения об ошибке
          Write_MCP23008(1,'c');
           Write_MCP23008(1,'r');
           Write_MCP23008(1,'c');
        } else {
          if ((family_code == 0x28) || (family_code == 0x22) || (family_code == 0x10)) { 
            // Если код семейства соответствует одному из известных... 
            // 0x10 - DS18S20, 0x28 - DS18B20, 0x22 - DS1822
            // проведём запрос scratchpad'а, считая по ходу crc
            onewire_send(0xBE); 
            uint8_t scratchpad[8];
            crc = 0;
            for (uint8_t i = 0; i < 8; i++) {
              uint8_t b = onewire_read();
              scratchpad[i] = b;
              crc = onewire_crc_update(crc, b);
            }
            if (onewire_read() != crc) {
              // Если контрольная сумма скретчпада не совпала.
               Write_MCP23008(1,'$');
              Write_MCP23008(1,'c');
           Write_MCP23008(1,'r');
           Write_MCP23008(1,'c');
          //    uart_hex(crc);
            } else {
              int16_t t = (scratchpad[1] << 8) | scratchpad[0];
              if (family_code == 0x10) { // 0x10 - DS18S20 
                // у DS18S20 значение температуры хранит 1 разряд в дробной части.
                // повысить точность показаний можно считав байт 6 (COUNT_REMAIN) из scratchpad
                t <<= 3;
                if (scratchpad[7] == 0x10) { // проверка на всякий случай
                  t &= 0xFFF0;
                  t += 12 - scratchpad[6];
                }
              } // для DS18B20 DS1822 значение по умолчанию 4 бита в дробной части
              // Вывод температуры
              uart_num(t);
            }            
          } else {
            // Неизвестное устройство
            Write_MCP23008(1,'?');
          }
        }
        //uart_newline();
      }      
     // uart_write('.');
    }
	do_1wire_or_nobody_home=0; //// ставим опять на ней ноль, чтобы еще раз проверить наличие устройства.
	sei(); /// разрешим прерывания, чтобы всё работало нормально.
	
			}

*/