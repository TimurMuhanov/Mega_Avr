#ifndef _OUTPUT_H_
#define _OUTPUT_H_ 1

#define lcd_adr 7

uint8_t f_hex(uint8_t ch)
{
	if (ch > 9)
	{
		ch += 0x37;
	}
	else
	{
		ch += 0x30;
	}
	return ch;
}

void f_hexout(uint8_t ch)
{
	lcdw_putbyte(lcd_adr,f_hex((ch&0xF0)>>4),LCD_DATA);
	lcdw_putbyte(lcd_adr,f_hex(ch&0x0F),LCD_DATA);
}

void f_debag(uint8_t b)
{
	lcdw_clear(lcd_adr);
	lcdw_gotoxy(lcd_adr,0,0);
	f_hexout(b);
	_delay_ms(100);
}

double f_for_output(double d)
{
	uint8_t ch = (uint8_t)trunc(d) + 0x30;
	lcdw_putbyte(lcd_adr,ch,LCD_DATA);
	d = (d-trunc(d))*10;
	return d;
}

void f_output_d(double t)
{
	int8_t deg = 0;
	if (t < 1)
	{
		while ((t < 1)&&(deg != -10))
		{
			t *= 10;
			deg--;
		}
	}
	else
	{
		while ((t>=10)&&(deg != 10))
		{
			t /= 10;
			deg++;
		}
	}
	
	t = f_for_output(t);
	lcdw_putbyte(lcd_adr,'.',LCD_DATA);	//.
	t = f_for_output(t);
	t = f_for_output(t);
	t = f_for_output(t);
	lcdw_putbyte(lcd_adr,'*',LCD_DATA);
	lcdw_putbyte(lcd_adr,'1',LCD_DATA);
	lcdw_putbyte(lcd_adr,'0',LCD_DATA);
	lcdw_putbyte(lcd_adr,'^',LCD_DATA);	//^
	if (deg < 0)
	{
		lcdw_putbyte(lcd_adr,'-',LCD_DATA);	//-
		deg = abs(deg);
	}
	else
	{
		lcdw_putbyte(lcd_adr,'+',LCD_DATA);
	}
	deg += 0x30;
	if (deg == 58)
	{
		deg = 0xF3;
	}
	lcdw_putbyte(lcd_adr,deg,LCD_DATA);
	lcdw_putbyte(lcd_adr,' ',LCD_DATA);
}

void f_output_p(double x)
{
	double t = x;
	if (t>1)
	{
		t = 1;
	}
	lcdw_clear(lcd_adr);
	t = f_for_output(t);
	t = f_for_output(t);
	t = f_for_output(t);
	lcdw_putbyte(lcd_adr,'%',LCD_DATA);
}

#endif