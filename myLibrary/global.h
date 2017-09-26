#ifndef _GLOBAL_H_
#define _GLOBAL_H_ 1

#define c_Hz F_CPU/1024/256

#define meter_freq 1
#define meter_volt 2
#define meter_batt 3
#define N_menu 3

double mass_meters[N_meters],mean,err;
uint8_t k;
uint8_t i_menu;

#endif