/*
 * GccFrequencyCounter.c
 *
 * Created: 24.09.2017 19:23:36
 * Author : shinobu
 */ 

#define F_CPU	16000000
#define TWI_CLK	10000
#define N_meters 200
#define c_Hz F_CPU/1024/256

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdlib.h>
#include "F:\_AtmelStudio\7.0\toolchain\avr8\avr8-gnu-toolchain\avr\include\my\mytwi.h"
#include "F:\_AtmelStudio\7.0\toolchain\avr8\avr8-gnu-toolchain\avr\include\my\myLCDtwi.h"

void f_tims_init(void)
{
	TCCR0 = 0x00;
	TIMSK = (0<<TOIE0)|(0<<TICIE1)|(1<<OCIE1A)|(0<<OCIE1B)|(0<<TOIE1)|(0<<OCIE2)|(1<<TOIE2);
	TCCR1A = 0x40;
	TCCR1B = 0x00;
	OCR1A = 0xFFFF;
	OCR1B = 0xFFFF;
	ICR1 = 0xFFFF;
	TCCR2 = 0x00;
	OCR2 = 0x80;
}

void f_tims_start(void)
{
	TCNT0 = 0x00;
	TCNT1 = 0x0000;
	TCNT2 = 0x00;
	TCCR0 |= 0x07;
	TCCR1B |= 0x07;
	TCCR2 |= 0x07;
}

void f_tims_stop(void)
{
	TCCR1B &= 0xF8;
	TCCR0 &= 0xF8;
	TCCR2 &= 0xF8;
}

double sum[N_meters],mean,err;
uint8_t k;

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

void f_debag(uint8_t b)
{
	lcdw_clear(7);
	lcdw_gotoxy(7,0,0);
	lcdw_putbyte(7,f_hex((b&0xF0)>>4),LCD_DATA);
	lcdw_putbyte(7,f_hex(b&0x0F),LCD_DATA);
	_delay_ms(100);
}

void f_output_d(double t)
{
	uint8_t ch = 0;
	int8_t deg = 0;
	if (t < 1)
	{
		while ((t < 1)&&(deg < 8))
		{
			t *= 10;
			deg--;
		}
	}
	else
	{
		while ((t>=10)&&(deg > -8))
		{
			t /= 10;
			deg++;
		}
	}
	
	ch = (uint8_t)trunc(t) + 0x30;
	t -= trunc(t);
	lcdw_putbyte(7,ch,LCD_DATA);
	lcdw_putbyte(7,0x2E,LCD_DATA);	//.
	t *= 10;
	ch = (uint8_t)trunc(t) + 0x30;
	t -= trunc(t);
	lcdw_putbyte(7,ch,LCD_DATA);
	t *= 10;
	ch = (uint8_t)trunc(t) + 0x30;
	t -= trunc(t);
	lcdw_putbyte(7,ch,LCD_DATA);
	t *= 10;
	ch = (uint8_t)trunc(t) + 0x30;
	lcdw_putbyte(7,ch,LCD_DATA);	//^
	lcdw_putbyte(7,0x5E,LCD_DATA);
	if (deg < 0)
	{
		lcdw_putbyte(7,0x2D,LCD_DATA);	//-
		deg = abs(deg);
	}
	deg += 0x30;
	lcdw_putbyte(7,deg,LCD_DATA);
	
}

ISR(TIMER2_OVF_vect)
{
	f_tims_stop();
	sum[k] = c_Hz*((TCNT0*0x10000)|(TCNT1));
	k++;
	f_tims_start();
}

void f_output(void)
{
	lcdw_gotoxy(7,0,0);
	f_output_d(mean);
	lcdw_putbyte(7,0x48,LCD_DATA);
	lcdw_putbyte(7,0x7A,LCD_DATA);
	lcdw_gotoxy(7,0,1);
	f_output_d(err);
	lcdw_putbyte(7,0x25,LCD_DATA);
}

void f_metering(void)
{
	f_tims_start();
	while (k < N_meters)			//  Здесь не проходит. вероятно не работает прерывание
	{
		f_debag(k);
	}
	f_tims_stop();
	mean = 0.000;
	for (uint8_t i = 0; i<N_meters; i++)
	{
		mean += sum[i];
	}
	mean /= N_meters;
	err = 0.000;
	for (uint8_t i = 0; i<N_meters; i++)
	{
		sum[i] -= mean;
		sum[i] *= sum[i];
		err += sum[i];
	}
	err = sqrt(err)*100/(N_meters-1)/mean;
}

int main(void)
{
	asm volatile ("push r16");	// Этот костыль оправдан, так как вероятно последние ячейки оперативной памяти не работают
	TWI_Init();
	LCDw_Init(7);
	f_tims_init();
	k = 0;
	DDRD = 0x00;
	PORTD = 0xFF;
	PORTB = 0x00;
	DDRB = 0x02;
	
	asm volatile ("sei");		// автоматически компилятор почему-то не разрешает прерывания
	f_metering();
	lcdw_clear(7);
	f_output();
	
    while (1) 
    {
    }
}