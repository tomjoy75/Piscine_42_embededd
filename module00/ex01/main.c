#include <avr/io.h>

int	main(void){
	DDRB |= (1<<PB0);
	// DDRB = Data direction register. 
	// PB0 which represent bit 0 has a value of 0b00000000
	// PB2 which represent bit 2 has a value of 0b00000010 
	// 1 << PB0 <=> 1 << 0 <=> 1
	// 1 << PB2 <=> 1 << 2 <=> 4
	// |= is OR which change the value of the chosen bit
	PORTB |=(1<<PB0);
	// Same as precedent
	while (1)
	{
	}
	// Infinite loop
	return (0);
}
