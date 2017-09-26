#ifndef _METERING_H_
#define _METERING_H_ 1

void f_metering(uint8_t meter, uint8_t second_arg)
{
	if (meter == meter_freq)
	{
		f_freq_init();
		f_freq_start();
		while (k < N_meters)
		{
			f_output_p(k/N_meters);
			_delay_ms(10);
		}
		f_freq_stop();
	}
	
	if (meter == meter_volt)
	{
		f_ADC_init();
		while (k < N_meters)
		{
			while (ADCSRA&0x40) {}
			f_ADC_start(second_arg);
			f_output_p(k/N_meters);
			_delay_ms(10);
		}
	}
	
	if (meter == meter_batt)
	{
		f_batt_init();
		f_batt_change_I(0x0000);
		f_batt_menu();
	}
	
	mean = 0.000;
	for (uint8_t i = 0; i<N_meters; i++)
	{
		mean += mass_meters[i];
	}
	mean /= N_meters;
	err = 0.000;
	for (uint8_t i = 0; i<N_meters; i++)
	{
		mass_meters[i] -= mean;
		mass_meters[i] *= mass_meters[i];
		err += mass_meters[i];
	}
	err = sqrt(err)*100/(N_meters-1)/mean;
}

#endif