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
#define end_of_mass 0x10
#define lcd_adr 7
#define meter_freq 1
#define meter_volt 2
#define N_menu 2

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include <stdlib.h>
//#include <string.h>
#include "..\myLibrary\mytwi.h"
#include "..\myLibrary\myLCDtwi.h"

#define _stop while (1) {}

void f_tims_init(void)
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

void f_tims_start(void)
{
	TCNT0 = 0x00;
	TCNT1 = 0x0000;
	TCNT2 = 0x00;
	TCCR2 |= 0x07;
	TCCR1B |= 0x07;
}

void f_tims_stop(void)
{
	TCCR1B &= 0xF8;
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
	lcdw_putbyte(lcd_adr,0x2E,LCD_DATA);	//.
	t = f_for_output(t);
	t = f_for_output(t);
	t = f_for_output(t);
	lcdw_putbyte(lcd_adr,0x5E,LCD_DATA);	//^
	if (deg < 0)
	{
		lcdw_putbyte(lcd_adr,0x2D,LCD_DATA);	//-
		deg = abs(deg);
	}
	deg += 0x30;
	if (deg == 58)
	{
		deg = 0xF3;
	}
	lcdw_putbyte(lcd_adr,deg,LCD_DATA);
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

ISR(TIMER2_OVF_vect)
{
	f_tims_stop();
	sum[k] = c_Hz*((TCNT0*0x10000)|(TCNT1));
	k++;
	f_tims_start();
}

void f_output_freq(void)
{
	lcdw_gotoxy(lcd_adr,0,0);
	f_output_d(mean);
	lcdw_putbyte(lcd_adr,0x48,LCD_DATA);	// Hz
	lcdw_putbyte(lcd_adr,0x7A,LCD_DATA);
	lcdw_gotoxy(lcd_adr,0,1);
	f_output_d(err);
	lcdw_putbyte(lcd_adr,0x25,LCD_DATA);	// %
}

void f_ADC_init(void)
{
	ADMUX = 0x40;	// AVcc (5V)
	//ADMUX = 0xC0;	// Internal 2.56V
	ADCSRA = 0x9F;	// ADC enable; interrupt flag cleared; optimum frequency = 50..200 kHz (from Internet)
}

void f_ADC_start(uint8_t num)
{
	ADMUX |= num;
	ADCSRA |= 0x40;
}

ISR(ADC_vect)
{
	sum[k] = (ADC&0x03FF);
	ADMUX &= 0xF0;
	k++;
}

void f_output_volt(void)
{
	lcdw_gotoxy(lcd_adr,0,0);
	f_output_d(mean);
	lcdw_gotoxy(lcd_adr,0,1);
	f_output_d(err);
	lcdw_putbyte(lcd_adr,0x25,LCD_DATA);	// %
}

void f_metering(uint8_t meter, uint8_t second_arg)
{
	if (meter == meter_freq)
	{
		f_tims_start();
		while (k < N_meters)
		{
			f_output_p(k/N_meters);
			_delay_ms(10);
		}
		f_tims_stop();
	}
	
	if (meter == meter_volt)
	{
		while (k < N_meters)
		{
			while (ADCSRA&0x40) {}
			f_ADC_start(second_arg);
			f_output_p(k/N_meters);
			_delay_ms(10);
		}
	}
	
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

const uint8_t mass_freq[]PROGMEM = {'F','r','e','q','M','e','t','e','r',end_of_mass};
const uint8_t mass_volt[]PROGMEM = {'V','o','l','t','M','e','t','e','r',end_of_mass};
//const uint8_t mass_volt[]PROGMEM = "VoltMeter";
//const uint8_t mass_menu[]PROGMEM = {&mass_freq,&mass_volt};

void f_out_mess(const uint8_t* mass)
{
	const uint8_t* i = mass;
	uint8_t ch = pgm_read_byte(i);
	while (ch != end_of_mass)
	{
		i++;
		lcdw_putbyte(lcd_adr,ch,LCD_DATA);
		ch = pgm_read_byte(i);
	}
}

uint8_t i_menu;

void main_menu(void)
{
	uint8_t t = (PIND&0x0C);
	_delay_ms(200);
	lcdw_clear(lcd_adr);
	if (t != 0x04)		// if you pressed 1, next point of menu
	{
		i_menu++;
		if (i_menu > N_menu)
		{
			i_menu = 1;
		}
		switch (i_menu)
		{
			case meter_freq:
				f_out_mess(mass_freq);
				break;
			case meter_volt:
				f_out_mess(mass_volt);
				break;
			default:
				break;
		}
		_delay_ms(200);
	} 
	else				// else run subprogram
	{
		switch (i_menu)
		{
			case meter_freq:
				f_metering(meter_freq,0);
				lcdw_clear(lcd_adr);
				f_output_freq();
				break;
			case meter_volt:
				f_metering(meter_volt,0);
				lcdw_clear(lcd_adr);
				f_output_volt();
				break;
			default:
				break;
		}
		k = 0;
	}
	do 	// waiting for clicking
	{
	} while ((PIND&0x0C) == 0x0C);
}

int main(void)
{
	DDRD = 0x00;
	PORTD = 0xFF;
	PORTB = 0x00;
	DDRB = 0x02;
	
	asm volatile ("push r16");	// the last memory cells don't work because China
	TWI_Init();
	LCDw_Init(lcd_adr);
	f_tims_init();
	f_ADC_init();
	k = 0;
	/*double l = 1/1;
	f_output_p(l);
	_stop;*/
	
	asm volatile ("sei");		// the compiler doesn't this automatically
	
	i_menu = 0;
	f_out_mess(mass_freq);
    while (1) 
    {
		main_menu();
    }
}
