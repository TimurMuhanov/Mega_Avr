#ifndef _MY_LCD_TWI_H_
#define _MY_LCD_TWI_H_ 1

#define _adr1602	0x70
#define _adr1602_0	0x70
#define _adr1602_1	0x72
#define _adr1602_2	0x74
#define _adr1602_3	0x76
#define _adr1602_4	0x78
#define _adr1602_5	0x7A
#define _adr1602_6	0x7C
#define _adr1602_7	0x7E

#define _lcd1602r	0x01

#define _lcd1602rs_set	0x01
#define _lcd1602rs_clr	0xFE
#define _lcd1602rw_set	0x02
#define _lcd1602rw_clr	0xFD
#define _lcd1602e_set	0x04
#define _lcd1602e_clr	0xFB
#define _lcd1602led_set	0x08
#define _lcd1602led_clr	0xF7

#define LCD_COMMAND  0
#define LCD_DATA     1

uint8_t _lcd1602last[8];

void lcdw_putnibble(uint8_t adr, uint8_t t)
{
	_lcd1602last[adr] |= _lcd1602e_set;
	TWI_transmit(_lcd1602last[adr]);
	_delay_us(50);
	_lcd1602last[adr] = t|(0x0F&_lcd1602last[adr]);
	TWI_transmit(_lcd1602last[adr]);
	_lcd1602last[adr] &= _lcd1602e_clr;
	TWI_transmit(_lcd1602last[adr]);
	_delay_us(50);
}

void lcdw_putbyte(uint8_t adr, char c, char rs)
{
	TWI_start();
	TWI_transmit(_adr1602|(adr<<1));
	if (rs==LCD_COMMAND) {
		_lcd1602last[adr] &= _lcd1602rs_clr;
		TWI_transmit(_lcd1602last[adr]);
	} else {
		_lcd1602last[adr] |= _lcd1602rs_set;
		TWI_transmit(_lcd1602last[adr]);
	}
	lcdw_putnibble(adr,c&0xf0);
	lcdw_putnibble(adr,(c&0x0f)<<4);
	TWI_stop();
}

void lcdw_clear(uint8_t adr)
{
	lcdw_putbyte(adr, 0x01, LCD_COMMAND);
	_delay_us(1500);
}

void lcdw_gotoxy(uint8_t adr, char col, char row)
{
	lcdw_putbyte(adr, ((0x40*row+col)|0x80), LCD_COMMAND);
}

uint8_t LCDw_Init(uint8_t adr)
{
	TWI_start();
	TWI_transmit(_adr1602|(adr<<1));
	TWI_transmit(0x00);
	_lcd1602last[adr] = 0x00;
	_delay_ms(15);
	lcdw_putnibble(adr,0x30);
	_delay_ms(4);
	lcdw_putnibble(adr,0x30);
	_delay_us(100);
	lcdw_putnibble(adr,0x30);
	_delay_ms(1);
	lcdw_putnibble(adr,0x20);
	TWI_stop();
	_delay_ms(1);
	lcdw_putbyte(adr, 0x28, LCD_COMMAND);
	_delay_ms(1);
	lcdw_putbyte(adr, 0x0C, LCD_COMMAND);
	_delay_ms(1);
	lcdw_putbyte(adr, 0x06, LCD_COMMAND);
	_delay_ms(1);
	lcdw_clear(adr);
	return 0;
}

void lcdw_led(uint8_t adr)
{
	TWI_start();
	TWI_transmit(_adr1602|(adr<<1));/*
	if (_lcd1602last[adr]&0x08)
	{
		_lcd1602last[adr]&=_lcd1602led_clr;
	} 
	else
	{
		_lcd1602last[adr]|=_lcd1602led_set;
	}*/
	_lcd1602last[adr]&=_lcd1602led_clr;
	_lcd1602last[adr]&=_lcd1602rw_clr;
	_lcd1602last[adr]|=_lcd1602led_set;
	_lcd1602last[adr]|=_lcd1602rw_set;
	TWI_transmit(_lcd1602last[adr]);
	TWI_stop();
}

#endif