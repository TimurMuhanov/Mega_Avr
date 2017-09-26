#ifndef _MY_INTERRUPT_H_
#define _MY_INTERRUPT_H_ 1

ISR(TIMER2_OVF_vect)
{
	f_freq_stop();
	mass_meters[k] = c_Hz*((TCNT0*0x10000)|(TCNT1));
	k++;
	f_freq_start();
}

ISR(ADC_vect)
{
	mass_meters[k] = (ADC&0x03FF);
	ADMUX &= 0xF0;
	k++;
}

#endif