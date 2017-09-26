/*
	TWBR - TWI Bit Rate Register
	TWCR – TWI Control Register
		Bit 7 – TWINT: TWI Interrupt Flag
		Bit 6 – TWEA: TWI Enable Acknowledge
		Bit 5 – TWSTA: TWI START Condition
		Bit 4 – TWSTO: TWI STOP Condition
		Bit 3 – TWWC: TWI Write Collision Flag
		Bit 2 – TWEN: TWI Enable
		Bit 0 – TWIE: TWI Interrupt Enable
	TWSR – TWI Status Register
		Bits 7:3 – TWSn: TWI Status Bit 7 [n = 7:3]
		Bits 1:0 – TWPSn: TWI Prescaler [n = 1:0] {00 - 1; 01 - 4; 10 - 16; 11 - 64}
	TWDR – TWI Data Register
		Bits 7:0 – TWDn: TWI Data [n = 7:0]
	TWAR – TWI (Slave) Address Register
		Bits 7:1 – TWAn: TWI (Slave) Address [n = 6:0]
		Bit 0 – TWGCE: TWI General Call Recognition Enable Bit
*/

#ifndef _MY_MYTWI_H_
#define _MY_MYTWI_H_ 1

#include <util/twi.h>
#include <util/delay.h>

#ifndef F_CPU
#warning "F_CPU not defined for my/mytwi.h"
#define F_CPU 1000000UL
#endif

#ifndef TWI_CLK
#warning "TWI_CLK not defined for my/mytwi.h"
#define TWI_CLK 1000
#endif

#if TWI_CLK>400000
#error "TWI_CLK max = 400000"
#endif

#if (TWI_CLK < (F_CPU/(16+2*64*255)))
#error "TWI_CLK min = F_CPU/(16+2*64*255))"
#endif

#define FTWCLK (((F_CPU/TWI_CLK)-16)/2)

#if (FTWCLK<=255)
#define TWPRH 0
#define TWPRL (unsigned char)FTWCLK
#endif

#ifndef TWPRH 
#if (FTWCLK<=1023)
#define TWPRH 1
#define TWPRL (unsigned char)(FTWCLK>>2)
#endif
#endif

#ifndef TWPRH 
#if FTWCLK<=4095
#define TWPRH 2
#define TWPRL (unsigned char)(FTWCLK>>4)
#endif
#endif

#ifndef TWPRH 
#define TWPRH 3
#define TWPRL (unsigned char)(FTWCLK>>6)
#endif

#define TWI_PORT PORTC
#define TWI_DDR_PORT DDRC
#define TWI_PIN_SCL 5
#define TWI_PIN_SDA 4

void TWI_Init(void){
	TWI_PORT |= (1<<TWI_PIN_SCL)|(1<<TWI_PIN_SDA);
	TWI_DDR_PORT &= ~((1<<TWI_PIN_SCL)|(1<<TWI_PIN_SDA));
	TWSR = (TWSR&0xF8)|TWPRL;
	TWBR = TWPRH;
}

void TWI_start(void){
	TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN)|(1<<TWEA);	// START
	while(!(TWCR & (1<<TWINT)));
}

void TWI_rstart(void){
	TWCR=(1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
	while(!(TWCR & (1<<TWINT)));
}

void TWI_transmit(unsigned char c){
	TWDR = c;
	TWCR = (1<<TWINT) | (1<<TWEN);
	while(!(TWCR & (1<<TWINT)));
}

unsigned char TWI_receive(unsigned char _TWI_r_End){
	if (!_TWI_r_End) {
		TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWEA);	// answ ack
	} 
	else {
		TWCR = (1<<TWINT)|(1<<TWEN);	// answ nack
	}
	while(!(TWCR & (1<<TWINT)));
	return TWDR;
}

void TWI_stop(void){
	TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWSTO);
	while(TWCR & (1<<TWSTO));
	_delay_ms(1);
}

#endif