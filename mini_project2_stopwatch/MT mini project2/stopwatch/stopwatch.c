/*
 * stopwatch.c
 *
 *  Created on: Jan 30, 2024
 *      Author: dell
 */
#include <avr/io.h>
#include <avr/interrupt.h>
#include "util/delay.h"
unsigned char SECONDS=0;
unsigned char MINUTES=0;
unsigned char HOURS=0;
int delay=1;//delay used for the multiplexing method
//to switch between the 7-segments very fast as all of them are always on

//display variables(units)
unsigned char sec_first_unit=0;//first unit from the lift to display seconds
unsigned char sec_second_unit=0;
unsigned char min_first_unit=0;//third unit from the lift to display minutes
unsigned char min_second_unit=0;
unsigned char hr_first_unit=0;//fifth unit from the lift to display hourS
unsigned char hr_second_unit=0;
//for interrupt buttons
char pause_counting=0;//if it equals1 it pauses the timer


/**FOR INT0*/
//PD2/INT0(reset button)
ISR(INT0_vect){
	_delay_ms(1);//for de-bouncing -it is as minimum as possible to prevent any flicking in the display
	//Initialize the display to 00:00:00
	 sec_first_unit=0;//first unit from the lift to display seconds
	 sec_second_unit=0;
	 min_first_unit=0;//third unit from the lift to display minutes
	 min_second_unit=0;
	 hr_first_unit=0;//fifth unit from the lift to display hourS
	 hr_second_unit=0;
	 //reset all variables
		SECONDS=0;
		MINUTES=0;
		HOURS=0;
	}
void INT0_Init(){
	MCUCR|=(1<<ISC01);//configure INT0 it with falling edge
	GICR|=(1<<INT0);// Enable external interrupt pin INT0
}

/**FOR INT1*/
//PD3/IN1(pause button)
ISR(INT1_vect){
	_delay_ms(1);//for de-bouncing -it is as minimum as possible to prevent any flicking in the display
	pause_counting=1;//pauses the timer as the variables stop incrementing
}
void INT1_Init(){
	//configure INT1 it with rising edge
	MCUCR|=(1<<ISC01);
	MCUCR|=(1<<ISC11);
	GICR|=(1<<INT1);// Enable external interrupt pin INT1
}

/**FOR INT2*/
//PB2/INT2(resume button)
ISR(INT2_vect){
	_delay_ms(1);//for de-bouncing -it is as minimum as possible to prevent any flicking in the display
	pause_counting=0;//Continue counting
}
void INT2_Init(){
	GICR|=(1<<INT2);// Enable external interrupt pin INT2
}


                    /**timer 1 configuration**/

ISR(TIMER1_COMPA_vect)
{
	if(	pause_counting==0)//checks if the pause button is pressed or not
{
	SECONDS++;
	if(SECONDS>=60){
		SECONDS=0;
		MINUTES++;//MINUTE HAVE PASSED
		if(MINUTES>=60){
			MINUTES=0;
			HOURS++;//AN HOUR HAVE PASSED
			if(HOURS>=100){
				HOURS=0;
			}
			}
		}
	// Update display units
	        sec_first_unit = SECONDS % 10;
	        sec_second_unit = SECONDS / 10;
	        min_first_unit = MINUTES % 10;
	        min_second_unit = MINUTES / 10;
	        hr_first_unit = HOURS % 10;
	        hr_second_unit = HOURS / 10;
	}
}





void timer1_ctc(){
		TCCR1A |=(1<<FOC1A);
		TCCR1B = (1<<WGM12)|(1<<CS11)|(1<<CS10);//set pre-scaler=64 to get T compare=1sec
		TCNT1 = 0;	//Set timer1 initial count to zero
		/*Frequency of the CPU=1Mhz ,N=64, T timer=1MHZ/64
		* T compare=1sec which mean compare match every 1 second
		* so compare match value = OCR1A= 15624*/
		OCR1A=15624;//set compare value
		TIMSK |=(1<<OCIE1A);//Timer1 Output Compare A match interrupt is
}

void RESET_DIPLAY(int num){
switch(num)
{
//SECONDS DISPLAY
case 1:
//seconds first unit (pin SEC1)
	PORTA|=(1<<PA0);//use the first  unit from the lift
	PORTC=(PORTC & 0xF0)|(sec_first_unit & 0x0F);//using insertion method to display the numbers
	_delay_us(delay);//so i can see the displayed number
	PORTA &= ~(1 << PA0); // Disable the first unit
break;
case 2:
//seconds second unit(pin SEC2)
	PORTA|=(1<<PA1);//use the second  unit
	PORTC=(PORTC & 0xF0)|(sec_second_unit & 0x0F);
	_delay_us(delay);
	PORTA &= ~(1 << PA1); // Disable the second unit
break;

	//MINUTES DISPLAY
	case 3:
		//minutes first unit(pin MIN1)
		PORTA|=(1<<PA2);//use the third  unit from the lift
		PORTC=(PORTC & 0xF0)|(min_first_unit & 0x0F);
		_delay_us(delay);
		PORTA &= ~(1 << PA2); // Disable the third unit
	break;
	case 4:
		//minutes second unit(pin MIN2)
		PORTA|=(1<<PA3);//use the fourth  unit
		PORTC=(PORTC & 0xF0)|(min_second_unit & 0x0F);
		_delay_us(delay);
		PORTA &= ~(1 << PA3); // Disable the fourth unit
	break;

		//HOURS DISPLAY
		case 5:
			//hours first unit(pin HR1)
			PORTA|=(1<<PA4);//use the fifth  unit from the lift
			PORTC=(PORTC & 0xF0)|(hr_first_unit & 0x0F);
			_delay_us(delay);
			PORTA &= ~(1 << PA4); // Disable the fifth unit
		break;
		case 6:
			//hours second unit(pin HR2)
			PORTA|=(1<<PA5);//use the sixth  unit
			PORTC=(PORTC & 0xF0)|(hr_second_unit & 0x0F);
			_delay_us(delay);
			PORTA &= ~(1 << PA5); // Disable the sixth unit
		break;

	}


}

int main(void){
	TCNT1 = 0;		/* Set timer1 initial count to zero */
	//ensures that the timer is properly configured before any interrupts occur.
	SREG |= (1<<7);//ENABLE GLOBAL INTEREPT IN I-BIT
	/**setting the pins*/
//PORTA each pin PA0->PA5 connected to transistor
	//SET PA0->PA5 AS OUTPUT PINS
	DDRA |= 0x3F; //00111111
	//SET PA0->PA5 TO ZERO AS initial value
	PORTA &=~0x3F;//00111111

//PORTC connected to the decoder
	//SET PC0->PC3 AS OUTPUT PINS
	DDRC |= 0xF; //1111
	PORTC &= ~0x0F; // 11110000


	/**setting interrupt buttons*/
//PD2/INT0(reset button)
	DDRD &=~(1<<PD2); //SET IT AS INPUT
	PORTD|=(1<<PD2);//WHEN PRESSED(1) INTERNAL PULL UP IS ACTIVTED
	INT0_Init();

//PD3/IN1(pause button)
	DDRD &=~(1<<PD3); //SET IT AS INPUT
	INT1_Init();

//PB2/INT2(resume button)
	DDRB &=~(1<<PB2); //SET IT AS INPUT
	PORTB|=(1<<PB2);//WHEN PRESSED(1) INTERNAL PULL UP IS ACTIVTED
	INT2_Init();

/**setting the timer***/
	timer1_ctc();//START THE TIMER
while(1)
{
	/**7-Segment Display**/
	PORTC &= 0xF0; // Reset the first 4 bits in PORTC

//seconds
RESET_DIPLAY(1);
RESET_DIPLAY(2);
		//minutes
		RESET_DIPLAY(3);
		RESET_DIPLAY(4);
					//hours
					RESET_DIPLAY(5);
					RESET_DIPLAY(6);


}
return 0;
}





