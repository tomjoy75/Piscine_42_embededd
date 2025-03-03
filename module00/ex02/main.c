#include <avr/io.h>

int	main(void){
	DDRB |= (1<<PB0);
	// 1<<PB0 = 1
	// DDRB |= (1<<PBO) sets first bit to 1
	DDRD &= ~(1<<PD2);
	// 1<<PD2 = 00000100
	// ~(1<<PD2) = 11111011
	// &= sets the third bit to 0
	while (1)
	{
		if ((PIND & (1<<PD2)) == 0)
			PORTB |= (1<<PB0);
		else
			PORTB &= ~(1<<PB0);
	}
	return (0);
}
