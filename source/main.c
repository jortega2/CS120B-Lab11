/*	Author: jorte057
 *  Partner(s) Name: Duke Pham dpham073@ucr.edu
 *	Lab Section:
 *	Assignment: Lab #  Exercise #
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#include "io.h"
#include "keypad.h"
#endif

volatile unsigned char TimerFlag = 0;

unsigned long _avr_timer_M = 1; // Start count from here, down to 0. Default 1 ms. 
unsigned long _avr_timer_cntcurr = 0;

void TimerSet(unsigned long M) {
	_avr_timer_M = M;
	_avr_timer_cntcurr = _avr_timer_M;
}

void TimerOn(){

TCCR1B = 0x0B;

OCR1A = 125; 

TIMSK1 = 0x02;

TCNT1 = 0;

_avr_timer_cntcurr = _avr_timer_M;

SREG |= 0x80;
}

void TimerOff(){
	TCCR1B = 0x00; // bit3bit1bit0: timer off

}

void TimerISR(){
	TimerFlag = 1;
}

ISR(TIMER1_COMPA_vect){
	_avr_timer_cntcurr--;
	if(_avr_timer_cntcurr == 0){
		TimerISR();
		_avr_timer_cntcurr = _avr_timer_M;
	
	}
}

typedef struct task {
	signed char state;
	unsigned long int period;
	unsigned long int elapsedTime;
	int (*TickFct)(int);
} task;

unsigned char led0_output = 0x00;
unsigned char led1_output = 0x00;
unsigned char pause = 0;

enum getButton {getButtonSystem};

int pauseButtonSMTick (int state) {
	switch (state) {
		case getButtonSystem : {
		unsigned char x;
		x = GetKeypadKey();
        	switch(x) {
                case '1' : PORTB = 0x01; break;
                case '2' : PORTB = 0x02; break;
                case '3' : PORTB = 0x03; break;
                case '4' : PORTB = 0x04; break;
                case '5' : PORTB = 0x05; break;
                case '6' : PORTB = 0x06; break;
                case '7' : PORTB = 0x07; break;
                case '8' : PORTB = 0x08; break;
                case '9' : PORTB = 0x09; break;
                case 'A' : PORTB = 0x0A; break;
                case 'B' : PORTB = 0x0B; break;
                case 'C' : PORTB = 0x0C; break;
                case 'D' : PORTB = 0x0D; break;
                case '*' : PORTB = 0x0E; break;
                case '0' : PORTB = 0x00; break;
                case '#' : PORTB = 0x0F; break;         
                default : PORTB = 0x1B; break;
        	}
		}
	}
	return state;
}

/*enum toggleLED0_States {toggleLED0_wait, toggleLED0_blink };

int toggleLED0SMTick(int state) {
	switch (state) {
		case toggleLED0_wait: state = !pause? toggleLED0_blink: toggleLED0_wait; break;
		case toggleLED0_blink: state = pause? toggleLED0_wait: toggleLED0_blink; break;
		default: state = toggleLED0_wait; break;
	}
	switch (state) {
		case toggleLED0_wait: break;
		case toggleLED0_blink:
			led0_output = (led0_output == 0x00) ? 0x01 : 0x00;
			break;
	}
	return state;
}

enum toggleLED1_States {toggleLED1_wait, toggleLED1_blink };

int toggleLED1SMTick (int state) {
		switch (state) {
			case toggleLED1_wait: state = !pause? toggleLED1_blink: toggleLED1_wait; break;
			case toggleLED1_blink: state = pause? toggleLED1_wait: toggleLED1_blink; break;
			default: state = toggleLED1_wait; break;
		}
		switch (state) {
			case toggleLED1_wait: break;
			case toggleLED1_blink:
				led1_output = (led1_output == 0x00) ? 0x01 : 0x00;
				break;
		}
		return state;
}

enum display_States {display_display };

int displaySMTick (int state) {
		unsigned char output;
		
		switch (state) {
			case display_display: state = display_display; break;
			default: state = display_display; break;
		}
		switch (state) {
			case display_display:
				output = led0_output | led1_output << 1;
				break;
		}
		PORTB = output;
		return state;
}*/	

int main(void) {
	DDRB = 0xFF; PORTB = 0x00;
	DDRA = 0x00; PORTA = 0xFF;
	DDRC = 0xF0; PORTC = 0x0F;
	//LCD_init();
	
	static task task1; //task2, task3, task4;
        task *tasks[] = { &task1};//, &task2, &task3, &task4 };
	const unsigned short numTasks = sizeof(tasks)/sizeof(task*);
	
	task1.state = getButtonSystem;
	task1.period = 50;
	task1.elapsedTime = task1.period;
	task1.TickFct = &pauseButtonSMTick;

	/*task2.state = toggleLED0_wait;
        task2.period = 500;
        task2.elapsedTime = task2.period;
        task2.TickFct = &toggleLED0SMTick;
	
	task3.state = toggleLED0_wait;
        task3.period = 1000;
        task3.elapsedTime = task3.period;
        task3.TickFct = &toggleLED1SMTick;

	task4.state = display_display;
        task4.period = 10;
        task4.elapsedTime = task4.period;
        task4.TickFct = &displaySMTick;*/
	
	TimerSet(10);
	TimerOn();
	
	unsigned short i;
    while (1) {

	for (i = 0; i < numTasks; i++ ) {
		if (tasks[i]->elapsedTime == tasks[i]->period){
			tasks[i]->state = tasks[i]->TickFct(tasks[i]->state);
			tasks[i]->elapsedTime = 0;
		}
		tasks[i]->elapsedTime += 10;
	}	
	while(!TimerFlag);
	TimerFlag = 0;
	
    }
    return 1;
}

