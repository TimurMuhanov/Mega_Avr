/*
 * GccFrequencyCounter.c
 *
 * Created: 24.09.2017 19:23:36
 * Author : shinobu
 */ 

/*
	Планируется добавить:
		зарядное устройство
			шим
			ацп 2 шт
		прошиватор
			spi
		общение по юарт
			другие устройства (433)
			компьютер
			возможность переключаться между ними
		вывести все лишнее в инклюды
*/

#define F_CPU	16000000
#define TWI_CLK	10000
#define N_meters 200

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include <stdlib.h>

#include "..\myLibrary\global.h"

//#include "..\myLibrary\tims.h"
#include "..\myLibrary\ADC.h"
#include "..\myLibrary\mytwi.h"

#include "..\myLibrary\myLCDtwi.h"
#include "..\myLibrary\output.h"

#include "..\myLibrary\volt.h"
#include "..\myLibrary\freq.h"
#include "..\myLibrary\batt.h"
#include "..\myLibrary\interrupt.h"
#include "..\myLibrary\metering.h"

#include "..\myLibrary\menu.h"

#define _stop while (1) {}

int main(void)
{
	DDRD = 0x00;
	PORTD = 0xFF;
	PORTB = 0x00;
	DDRB = 0x02;
	
	asm volatile ("push r16");	// the last memory cells don't work because China
	TWI_Init();
	LCDw_Init(lcd_adr);
	
	/*double l = 1/1;
	f_output_p(l);
	_stop;*/
	
	asm volatile ("sei");		// the compiler doesn't this automatically
	
	k = 0;
	i_menu = 0;
	f_out_mess(mass_freq);
    while (1) 
    {
		main_menu();
    }
}
