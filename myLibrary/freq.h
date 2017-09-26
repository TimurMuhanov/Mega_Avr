#ifndef _MY_FREQ_H_
#define _MY_FREQ_H_ 1

void f_output_freq(void)
{
	lcdw_gotoxy(lcd_adr,0,0);
	f_output_d(mean);
	lcdw_putbyte(lcd_adr,'H',LCD_DATA);	// Hz
	lcdw_putbyte(lcd_adr,'z',LCD_DATA);
	lcdw_gotoxy(lcd_adr,0,1);
	f_output_d(err);
	lcdw_putbyte(lcd_adr,'%',LCD_DATA);	// %
}

#endif