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
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#include "io.h"
#include "keypad.h"
#endif

typedef struct _tast {
	signed char state;
	unsigned long int period;
	unsigned long int elapsedTime;
	int (*TickFct)(int);
} task;

unsigned char led0_output = 0x00;
unsigned char led1_output = 0x00;
unsigned char pause = 0;

enum pauseButtonSM_States {pauseButton_wait, pauseButton_press, pauseButton_release};

int pauseButtonSMTick (int state) {
	unsigned char press = ~PINA & 0x01;

	switch (state) {
		case pauseButton_wait:
			state = press = 0x01? pauseButton_press: pauseButton_wait; break;
		case pauseButton_press:
			state = pauseButton_release; break;
		case pauseButton_release:
			state = press == 0x00? pauseButton_wait: pauseButton_press; break;
		default: state = pauseButton_wait; break;
	}
	switch (state) {
		case pauseButton_wait: break;
		case pauseButton_press:
			pause = (pause == 0) ? 1 : 0;
			break;
		case pauseButton_release: break;
	}
	return state;
}

enum toggleLED0_States {toggleLED0_wait, toggleLED0)blink };

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

enum display_States (display_display );

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
}

/*unsigned char GetSingleButton(unsigned char row, unsigned char col){
	//set b3..b0 outputs to 1 except the bit at positino 'col',
	//ensuring b7..b4 are not modified
	B = (B | 0x0F) & ~(1 << col);

	//now read the input pin at position 'row'. If the button at 
	//row/col is pressed, it will go from pull-up state (1) to 0
	return ((A & (1 << row)) == 0 ? 1 : 0;
}

unsigned char ScanKeypad(){
	unsigned char i, j;
	for (i = 0; i < 4; i++){
		for (j = 0; j < 4; j++){
			if (GetSingleButton(i,j)){
				return (i * 4) + j + 1;
			}
		}
	}
	return 0;
}*/
	

int main(void) {
    /* Insert DDR and PORT initializations */
	//unsigned char x;
	DDRB = 0xFF; PORTB = 0x00;
	DDRA = 0x00; PORTA = 0xFF;
	//DDRC = 0xF0; PORTC = 0x0F;
	//LCD_init();
    /* Insert your solution below */	
	static _task, task1, tas2, task3, task4;
	task1.state = start;
	task1.period = 50;
	task1.elapsedTime = task1.period;
	task1.TickFct = &pauseButtonToggleSMTick;

	task2.state = start;
        task2.period = 500;
        task2.elapsedTime = task2.period;
        task2.TickFct = &toggleLED0SMTick;
	
	task3.state = start;
        task3.period = 1000;
        task3.elapsedTime = task3.period;
        task3.TickFct = &toggleLED1SMTick;

	task4.state = start;
        task4.period = 10;
        task4.elapsedTime = task4.period;
        task4.TickFct = &displaySMTick;
	
	TimerSet(/*GCD*/);
	TimerOn;
	
	unsigned short i;
    while (1) {
	for (i = 0; i < numTasks; i++ ) {
		if (tasks[i]->elapsedTime == tasks[i]->period){
			task[i]->state = tasks[i]->TickFct(tasks[i]->state);
			tasks[i]->elapsedTime = 0;
		}
		tasks[i]->elapsedTime += /*GCD*/;
	}	
	while(!TimerFlag);
	TimerFlag = 0;
	

	/*x = GetKeypadKey();
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
	}*/
    }
    return 1;
}
