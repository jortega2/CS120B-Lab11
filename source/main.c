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
#include "io.h"
#include <avr/interrupt.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
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

/*enum getButton {getButtonSystem};

int keyPad (int state) {
	switch (state) {
		case getButtonSystem : {
		unsigned char x;
		x = GetKeypadKey();
        	switch(x) {
		case '\0' : PORTB = 0x1F; break;
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
}*/

enum legendary {wait, press, write};

int legendary_Tick(int state) {
	static unsigned char cnt;
	switch (state) {
		case wait:
			cnt = 0;
			PORTB = 0x01;
			LCD_ClearScreen();
			if ((~PINA & 0x01) == 1){
				state = press;
			} else  {
				state = wait;
			}
		break;
		case press:
			PORTB = 0x02;
			if ((~PINA & 0x01) == 1){
				state = press;
			} else {
			state = write;
			}
		break;
		case write:
			PORTB = 0x04;	
			state = write;
		break;
	}
	switch (state) {
		case wait:
			LCD_ClearScreen();
			break;
		case press:
			break;
		case write:
			cnt++;	
			if (cnt == 1) {
				LCD_ClearScreen();
				LCD_DisplayString(1, "CS120B is Legend");
			}else if (cnt == 2) {
				LCD_ClearScreen();
				LCD_DisplayString(1, "S120B is Legend.");
			}else if (cnt == 3) {
                                LCD_ClearScreen();
                                LCD_DisplayString(1, "120B is Legend..");
                        }else if (cnt == 4) {
                                LCD_ClearScreen();
                                LCD_DisplayString(1, "20B is Legend...");
                        }else if (cnt == 5) {
                                LCD_ClearScreen();
                                LCD_DisplayString(1, "0B is Legend... ");
                        }else if (cnt == 6) {
                                LCD_ClearScreen();
                                LCD_DisplayString(1, "B is Legend... w");
                        }else if (cnt == 7) {
                                LCD_ClearScreen();
                                LCD_DisplayString(1, " is Legend... wa");
                        }else if (cnt == 8) {
                                LCD_ClearScreen();
                                LCD_DisplayString(1, "is Legend... wai");
                        }else if (cnt == 9) {
                                LCD_ClearScreen();
                                LCD_DisplayString(1, "s Legend... wait");
                        }else if (cnt == 10) {
                                LCD_ClearScreen();
                                LCD_DisplayString(1, " Legend... wait ");
                        }else if (cnt == 11) {
                                LCD_ClearScreen();
                                LCD_DisplayString(1, "Legend... wait f");
                        }else if (cnt == 12) {
                                LCD_ClearScreen();
                                LCD_DisplayString(1, "egend... wait fo");
                        }else if (cnt == 13) {
                                LCD_ClearScreen();
                                LCD_DisplayString(1, "gend... wait for");
                        }else if (cnt == 14) {
                                LCD_ClearScreen();
                                LCD_DisplayString(1, "end... wait for ");
                        }else if (cnt == 15) {
                                LCD_ClearScreen();
                                LCD_DisplayString(1, "nd... wait for i");
                        }else if (cnt == 16) {
                                LCD_ClearScreen();
                                LCD_DisplayString(1, "d... wait for it");
                        }else if (cnt == 17) {
                                LCD_ClearScreen();
                                LCD_DisplayString(1, "... wait for it ");
                        }else if (cnt == 18) {
                                LCD_ClearScreen();
                                LCD_DisplayString(1, ".. wait for it D");
                        }else if (cnt == 19) {
                                LCD_ClearScreen();
                                LCD_DisplayString(1, ". wait for it DA");
                        }else if (cnt == 20) {
                                LCD_ClearScreen();
                                LCD_DisplayString(1, " wait for it DAR");
                        }else if (cnt == 21) {
                                LCD_ClearScreen();
                                LCD_DisplayString(1, "wait for it DARY");
                        }else if (cnt == 22) {
                                LCD_ClearScreen();
                                LCD_DisplayString(1, "ait for it DARY!");
                        }
			break;
	}
			
	return state;
}

/*enum toggleLED1_States {toggleLED1_wait, toggleLED1_blink };

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
	DDRC = 0xFF; PORTC = 0x00;
	DDRD = 0xFF; PORTD = 0x00;
	LCD_init();
	
	static task task2; //task2, task3, task4;
        task *tasks[] = { &task2};//, &task2, &task3, &task4 };
	const unsigned short numTasks = sizeof(tasks)/sizeof(task*);
	
	/*task1.state = getButtonSystem;
	task1.period = 50;
	task1.elapsedTime = task1.period;
	task1.TickFct = &keyPad;*/

	task2.state = wait;
        task2.period = 500;
        task2.elapsedTime = task2.period;
        task2.TickFct = &legendary_Tick;
	
	/*task3.state = toggleLED0_wait;
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

