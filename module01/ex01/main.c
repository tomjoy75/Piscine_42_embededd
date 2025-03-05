#include "main.h"

// int	main(void){

// 	DDRB |= (1<<PB1); // activate data register for Port B on PB1
// 	TCCR1B |= (1<<CS12); // set prescaler to 256
// 	TCCR1A |= (1<<COM1A0); // toggle each comparison with OCR1A 
// 	TCNT1 = 0; // set timer to 0
// 	uint32_t target_timer = NEW_FREQ / 2 - 1;
// 	while (1){
// 		if (TCNT1 >= target_timer){
// 	//		PORTB ^= (1<<PB1); // XOR Operation to invert the output
// 			TCCR1C ^= (1<<FOC1A);
// //			PINB ^= (1<<PB1);	
// 			TCNT1 = 0;
// 		}
// 	}
// 	return (0);
// }

int	main(void){
	DDRB |= (1<<PB1); // Configure PB1 as output	
	//TCCR1A = 0;
	TCCR1B = (1<<WGM12); // CTC mode
	OCR1A = 7811; // (F_CPU / Prescaler * Temps) - 1
	// (before prescale)F_CPU = 16Mhz = 16000000 Hz
	// (after prescale) F_CPU = 16000000 / 1024  = 15624
	// (2 operation by sec) OCR1A = 15624 / 2 -1 = 7811
	TCCR1A |= (1<<COM1A0); // Activate toggle OC1A to invert automatically PB1  
	TCCR1B |= (1<<CS12) | (1<<CS10); // prescale at /1024 and launch the timer
	while (1)
	{}

	return(0);
}