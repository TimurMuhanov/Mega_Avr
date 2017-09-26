#ifndef _MY_FREQ_H_
#define _MY_FREQ_H_ 1

void f_freq_init(void)
{
	TCCR0 = 0x07;
	TIMSK = (0<<TOIE0)|(0<<TICIE1)|(1<<OCIE1A)|(0<<OCIE1B)|(0<<TOIE1)|(0<<OCIE2)|(1<<TOIE2);
	TCCR1A = 0x40;
	TCCR1B = 0x00;
	TCCR2 = 0x00;
	
	OCR1A = 0xFFFF;	// not necessarily
	OCR1B = 0xFFFF;
	ICR1 = 0xFFFF;
	OCR2 = 0x80;
}

void f_freq_start(void)
{
	TCNT0 = 0x00;
	TCNT1 = 0x0000;
	TCNT2 = 0x00;
	TCCR2 |= 0x07;
	TCCR1B |= 0x07;
}

void f_freq_stop(void)
{
	TCCR1B &= 0xF8;
	TCCR2 &= 0xF8;
}

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