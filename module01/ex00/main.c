#include <avr/io.h>
#include <util/delay.h>

#define DELAY 500

int	main(void){

	DDRB |= (1<<PB1); // activate data register for Port B on PB1
	while (1){
		PORTB ^= (1<<PB1); // XOR Operation to invert the output
		_delay_ms(DELAY); 
	}
	return (0);
}
