// #include "main.h"

// int	main(void){

// 	DDRB |= (1<<PB1); // activate data register for Port B on PB1
// 	TCCR1B |= (1<<CS12); // set prescaler to 256
// 	TCNT1 = 0; // set timer to 0
// 	uint32_t target_timer = NEW_FREQ / 2 - 1;
// 	while (1){
// 		if (TCNT1 >= target_timer){
// 			PORTB ^= (1<<PB1); // XOR Operation to invert the output
// 			TCNT1 = 0;
// 		}
// 	}
// 	return (0);
// }

#include "main.h"

void	wait(uint32_t delay) {
	for (volatile uint32_t i = 0; i < delay; i++);
}

uint32_t	compute_delay(void){
	double		iter_time = 30.0 / (double)F_CPU; // Time for 1 iteration i seconds
	uint32_t	delay = (uint32_t)((DELAY / 1000.0) / iter_time); // Compute in ns
	return (delay);
}

int		main(void){
	uint32_t	delay = compute_delay();
	
	DDRB |= (1<<PB1); // activate data register for Port B on PB1
	while(1){
 		PORTB ^= (1<<PB1); // XOR Operation to invert the output
		wait(delay);
	}
	return (0);
}
