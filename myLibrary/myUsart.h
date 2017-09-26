
#ifndef _MY_MYUSART_H_
#define _MY_MYUSART_H_ 1
/*
#ifndef F_CPU
#warning "F_CPU not defined for my/myUsart.h"
#define F_CPU 1000000UL
#endif

#ifndef BAUD
#warning "BAUD not defined for my/myUsart.h"
#define BAUD 2400
#endif
*/
#define MYUBRR F_CPU/16/BAUD-1

void USART_Init(unsigned int ubrr)
{
	/*Set baud rate */
	UBRR0H = (unsigned char)(ubrr>>8);
	UBRR0L = (unsigned char)ubrr;
	/*Enable receiver and transmitter */
	UCSR0B = (1<<RXEN0)|(1<<TXEN0);
	/* Set frame format: 8data, 1stop bit */
	UCSR0C = (0<<USBS0)|(3<<UCSZ00);
}

void USART_Transmit( unsigned char data )
{
	/* Wait for empty transmit buffer */
	while ( !( UCSR0A & (1<<UDRE0)) );
	/* Put data into buffer, sends the data */
	UDR0 = data;
}
/*
unsigned char USART_Receive( void )
{
	// Wait for data to be received
	while ( !(UCSR0A & (1<<RXC0)) );
	// Get and return received data from buffer
	return UDR0;
}
unsigned char USART_Receive_PC( void )
{
	unsigned char th = USART_Receive();
	if (th>58) {th=th-55;} else {th=th-48;}
	th = th<<4;
	unsigned char tl = USART_Receive();
	if (tl>58) {tl=tl-55;} else {tl=tl-48;}
	return th|tl;//(th<<4)|tl;
}
*/
unsigned char USART_Receive_PC( void )
{
	// Wait for data to be received
	while ( !(UCSR0A & (1<<RXC0)) );
	// Get and return received data from buffer
	//	ѕришлось дублировать, потому что один байт
	//	по-человечески отправить не удаетс€
	unsigned char t = UDR0;
	if (t>58) {t=t-55;} else {t=t-48;}
	unsigned char bb = (t<<4);
	while ( !(UCSR0A & (1<<RXC0)) );
	t = UDR0;
	if (t>58) {t=t-55;} else {t=t-48;}
	bb |= t;
	return bb;
}

#endif