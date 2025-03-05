#include "main.h"



int	main(void){
	DDRB |= (1<<PB1); // Configure PB1 as output	
	
	TCCR1A |= (1 << COM1A1) | (1 << WGM11); // Mode Fast PWM
	TCCR1B |= (1 << WGM12) | (1 << WGM13); 
	ICR1 = 7811; // Fix the period
	OCR1A = 781; // Cyclic ratio 10%
	TCCR1B |= (1<<CS12) | (1<<CS10); // prescale at /1024 and launch the timer
	while (1)
	{}

	return(0);
}