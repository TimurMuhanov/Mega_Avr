#ifndef _MY_ADC_H_
#define _MY_ADC_H_ 1

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

#endif