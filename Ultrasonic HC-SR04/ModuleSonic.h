/*
Title : Ultrasonic Sensor
Author : R M Lakruwan @ Noone
Date : 24 May 2014
Compatibality : Module HC-SR04
version : 1.0

Description : 
*#include <ModuleSonic.h>
				* ModuleSonic.h
				************************************************************************
				*initSonic(void)				-DDR on TRIGER,LED and ECHO
				*startTimer1(void)				-Start Timer Counter 1
				*endTimer1(void)				-Disable Timer Counter 1
				*onExtINT0(void)				-Enable External Inttrupt INT0 for Echo
				*offExtINT0(void)				-Disable External Inttrupt INT0 for Echo
				*pulseTrigger(void)				-Trigger a pulse For module
				************************************************************************
				*EchoPin-2		TrigPin-3		LEDPin-4(testOnly)
				************************************************************************
*/

#include <avr/io.h>
#include <avr/interrupt.h>

#ifndef F_CPU
#warning "F_CPU not defined (Defaulted to 16000000UL (8Mhz)"
#define F_CPU 16000000UL
#endif

//****** I/O pin Sonic Contrall*****//
#define 	EchoPin		 2	//Int0 triger for pin change interrrupt
#define 	TrigPin		 3	//Int1 with OC2B TC2 - 8-bit Timer/Counter2 with PWM and Asynchronous Operation	
//#define 	LEDPin		 4	//Reaction measure			NC
//**********************************//

void onExtINT0(void){
//Enable External Inttrupt INT0 for Echo
	EICRA |= (1 << ISC01);    // set INT0 to trigger on Rising Edge 
	EIMSK |= (1 << INT0);     // Turns on INT0
	SREG |= (1<<INT0);
	
	sei();                    // turn on interrupts		
}

void initSonic(void){
//Enable DDR for Echo,Trigeer and test LED
	DDRD = (1<<TrigPin);// | (1<<LEDPin);		// Set Pins As Output
	DDRD &= ~(1<<EchoPin);					//Echo pin as Input	
}

void offExtINT0(void){
//Disable External Inttrupt INT0 for Echo
	EIMSK &= ~_BV(INT0);				// Disable interrupt on INT0
}

void pulseTrigger(void){
//Trigger a pulse For module
	PORTD &= ~(1<<TrigPin);		// Set Trigger Pin To 0
	_delay_us(10);				// Wait
	PORTD |= (1<<TrigPin);		// Set Trigger pin To 1
	_delay_us(10);				// Wait
	PORTD &= ~(1<<TrigPin);		// Set Trigger Pin To 					
}

void startTimer1(void){
//Start Timer Counter 1
	TCNT1=0;							//Set Initial Timer value
	TCCR1B|=(1<<CS10);					//Start timer without prescaller
	sei();								//Enable global interrutps	
}

void endTimer1(void){
//Disable Timer Counter 1
	TCCR1B&=~(1<<CS10);					//Disable timer 
}