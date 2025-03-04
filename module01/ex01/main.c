#include "main.h"

int	main(void){

	DDRB |= (1<<PB1); // activate data register for Port B on PB1
	TCCR1B |= (1<<CS12); // set prescaler to 256
	TCCR1A |= (1<<COM1A0); // toggle each comparison with OCR1A 
	TCNT1 = 0; // set timer to 0
	uint32_t target_timer = NEW_FREQ / 2 - 1;
	while (1){
		if (TCNT1 >= target_timer){
	//		PORTB ^= (1<<PB1); // XOR Operation to invert the output
			TCCR1C ^= (1<<FOC1A);
//			PINB ^= (1<<PB1);	
			TCNT1 = 0;
		}
	}
	return (0);
}
