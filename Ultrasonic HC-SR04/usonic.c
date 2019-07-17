/*
Title : Ultrasonic Sensor
Author : R M Lakruwan @ Noone
Date : 24 May 2014
Compatibality : Module HC-SR04
version : 1.0

*/

//*** Libraries **** //
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <UART.h>
				/***********UART.h*************
				*
				*serialOutString(char* Message)	-serialOutString("Array") without line break
				*serialInit8N1()				-serial Initialization
				*serialOutLine(char* Message)	-serialOutLine("Just String")
				*serialInChar()					-serialInChar()
				*
				********************************/
				
//********************//
#include <ModuleSonic.h>
				/***********ModuleSonic.h*************
				*
				*initSonic(void)				-DDR on TRIGER,LED and ECHO
				*startTimer1(void)				-Start Timer Counter 1
				*endTimer1(void)				-Disable Timer Counter 1
				*onExtINT0(void)				-Enable External Inttrupt INT0 for Echo
				*offExtINT0(void)				-Disable External Inttrupt INT0 for Echo
				*pulseTrigger(void)				-Trigger a pulse For module
				*
				*EchoPin-2		TrigPin-3		LEDPin-4(testOnly)
				********************************/
				
//********************//

#define F_CPU 8000000UL
#define BAUD_RATE 9600

volatile uint8_t pingState 	= 0;
volatile uint8_t echoCm 	= 0;
volatile char snum[8];

int main(void){
	serialInit8N1();			//Initialize serial
	initSonic();				//Initialize Sonic pins DDR
		
	while(1){
		
		switch (pingState){
			//Send Triger pulse 10 us long
			case 0:
					onExtINT0();					//Enable External Inttrupt INT0	
					pulseTrigger();					//Trigger a pulse For module
					pingState=1;	
				
			case 1:	
					while(!(PIND & (1<<EchoPin)));	//loop until Echo Begin
				
			case 2:											
					startTimer1();						//Intrrupt timer Start
						while((PIND & (1<<EchoPin)));	//loop until  Echo Over
					endTimer1();						//Intrrupt timer Stop
	
					echoCm = (TCNT1)/(16*58);			// Count echocms
					
				//Testing Idicater LED	
				//	PORTD |= (1<<LEDPin);
				//	_delay_ms(250);
				//	PORTD &= ~(1<<LEDPin);
				//	_delay_ms(250);
					
				// convert 123 to string [buf]
					itoa(echoCm, snum, 10);
					
					serialOutString("The Distance is : ");
					serialOutLine(snum);	
					serialOutLine("-------------------");	
					
					pingState = 3;					// Go To State resettting mode
					
				break;
	
			case 3:
					_delay_us(50);
					pingState=0;
					_delay_ms(100);
					
		}
	}
}

ISR(INT0_vect) {
	pingState=2;						//change Pinstate for Start Timer	
	offExtINT0();						// Disable interrupt on INT0
}
