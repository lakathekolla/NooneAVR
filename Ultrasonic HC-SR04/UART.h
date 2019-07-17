#include <avr/io.h>
#define F_CPU 16000000UL
#define BAUD_RATE 9600
#define BAUD (((F_CPU / (BAUD_RATE * 16UL))) - 1)


#ifndef F_CPU
#warning "F_CPU not defined (Defaulted to 16000000UL (8Mhz)"
#define F_CPU 16000000UL
#endif

#ifndef BAUD_RATE
#warning "BAUD_RATE not defined (Defaulted to 9600)"
#define BAUD_RATE 9600
#endif



void serialOutChar(unsigned char Message){
	while(!( UCSR0A & (1<<UDRE0)));
	UDR0 = Message;
}

void serialOutString(char* Message){
	while(*Message != 0x00){
		serialOutChar(*Message);
		Message++;
	}
}

void serialOutLine(char* Message){
	serialOutString(Message);
	serialOutString("\r\n");
}

unsigned char serialInChar(void){
	while(!(UCSR0A & (1<<RXC0)));
	return UDR0;
}

void serialInit8N1(void){
	UBRR0 = BAUD;
	UCSR0B = (1<<RXEN0)|(1<<TXEN0);
	UCSR0C = (3<<UCSZ00);
	DDRD = (1<<PIND1);
}