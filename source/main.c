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
#endif


unsigned char GetSingleButton(unsigned char row, unsigned char col){
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
}

int main(void) {
    /* Insert DDR and PORT initializations */
	DDRA = 0x00; PORTA = 0xFF;
	DDRC = 0xFF; PORTC = 0x00;
	DDRD = 0xFF; PORTD = 0x00;
	LCD_init();
    /* Insert your solution below */
    while (1) {

    }
    return 1;
}
