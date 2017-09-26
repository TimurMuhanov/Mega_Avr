#ifndef _MY_BATT_H_
#define _MY_BATT_H_ 1

#define f_batt_start	TCCR1B |= 1;
#define f_batt_stop		TCCR1B &= 0xF8;

/*
	В первую очередь сделаем меню для аккумуляторов
		Li-ion	(1 банка)
		Cd-Mn	(n банок)
*/

void f_batt_init(void)
{
	// init adc
	ADMUX = 0x40;	// AVcc (5V)
	//ADMUX = 0xC0;	// Internal 2.56V
	ADCSRA = 0x9F;	// ADC enable; interrupt flag cleared; optimum frequency = 50..200 kHz (from Internet)
	// init pwm
	TIMSK = (0<<TOIE0)|(0<<TICIE1)|(1<<OCIE1A)|(0<<OCIE1B)|(0<<TOIE1)|(0<<OCIE2)|(1<<TOIE2);
	TCCR1A = 0x33;	// fast pwm
	TCCR1B = 0x18;
}

void f_batt_change_I(uint16_t _num)
{
	OCR1A = _num;
	OCR1B = _num;
}

void f_batt_read(uint8_t num)	// use adc and interrupt
{
	
}

void f_output_batt()
{
	
}

void f_batt_menu(void)
{
	
}

#endif