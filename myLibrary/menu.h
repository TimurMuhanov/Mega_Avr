#ifndef _MENU_H_
#define _MENU_H_ 1

#define end_of_mass 0x10

const uint8_t mass_freq[]PROGMEM = {'F','r','e','q','M','e','t','e','r',end_of_mass};
const uint8_t mass_volt[]PROGMEM = {'V','o','l','t','M','e','t','e','r',end_of_mass};
const uint8_t mass_batt[]PROGMEM = {'B','a','t','t','e','r','y',' ','C','h','a','r','g','e','r',end_of_mass};
//const uint8_t mass_volt[]PROGMEM = {0xC3,0xA8,0xCF,' ',' ',' ',' ',' ',' ',end_of_mass};
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
			case meter_batt:
				f_out_mess(mass_batt);
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
			case meter_batt:
				f_metering(meter_batt,0);
				lcdw_clear(lcd_adr);
				f_output_batt();
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

#endif