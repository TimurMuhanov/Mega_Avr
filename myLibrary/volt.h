#ifndef _MY_VOLT_H_
#define _MY_VOLT_H_ 1

void f_output_volt(void)
{
	lcdw_gotoxy(lcd_adr,0,0);
	f_output_d(mean);
	lcdw_gotoxy(lcd_adr,0,1);
	f_output_d(err);
	lcdw_putbyte(lcd_adr,'%',LCD_DATA);	// %
}

#endif