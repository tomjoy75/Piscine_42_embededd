#include "main.h"

int	is_pushed(char pin){

	if ((PIND & (1<<pin)) == 0)
	{
		_delay_ms(BOUNCE_DELAY); // Protection for the bounce
		if ((PIND & (1<<pin)) == 0)
			return (1);
	}
	return (0);
}

int	main(void){
	char sw1_flag = 0;
	char sw2_flag = 0;
	DDRB |= (1<<PB1); // Configure PB1 as output	
	DDRD &= ~(1<<PD2); // Configure SW1 & SW2 as input
	DDRD &= ~(1<<PD4);
	
	TCCR1A |= (1 << COM1A1) | (1 << WGM11); // Mode Fast PWM
	TCCR1B |= (1 << WGM12) | (1 << WGM13); 
	ICR1 = PERIOD; 					// Fix the period
	OCR1A = PERIOD / 10; 			// Cyclic ratio 10%
	TCCR1B |= (1<<CS12) | (1<<CS10); // prescale at /1024 and launch the timer
	while (1)
	{
		while (is_pushed(PD2)){
			if (!sw1_flag){
				OCR1A += PERIOD / 10;
				if (OCR1A > ICR1)
					OCR1A = ICR1;
				sw1_flag = 1;
			}
		}
		
		while (is_pushed(PD4)){
			if (!sw2_flag){
				OCR1A -= PERIOD / 10;
					if (OCR1A < PERIOD / 10)
						OCR1A = PERIOD / 10;
				sw2_flag = 1;
			}
		}
		sw1_flag = 0;
		sw2_flag = 0;
	}
	return(0);
}