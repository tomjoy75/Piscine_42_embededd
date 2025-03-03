#include <avr/io.h>
#include <util/delay.h>

# define LIGHT_OFF 0
# define BOUNCE_DELAY 50

int	is_pushed(char pin){

	if ((PIND & (1<<pin)) == 0)
	{
		_delay_ms(BOUNCE_DELAY); // Protection for the bounce
		if ((PIND & (1<<pin)) == 0)
			return (1);
	}
	return (0);
}

// int	is_lighted(char port){
// 	if ((PINB & (1<<port)) == 1)
// 	       return (1);
// 	return (0);	
// }

void	actualise(char i){
	PORTB &= ~(0b00000111);  // Erase PB0 to PB2
	PORTB &= ~(1 << PB4);	// Erase PB4

	PORTB |= (i & 0b00000111);  // Actualise LEDs
	if (i & 0b00001000){ // If the fourth bit is 1
		PORTB |= 0b00010000; // Light D4
//		PORTB &= 0b00010111; // Erase fourth bit
	}
}

int	main(void){

	char sw1_flag = 0;
	char sw2_flag = 0;
	// DDRB |= (1<<PB0);
	// DDRB |= (1<<PB1);
	// DDRB |= (1<<PB2);
	// DDRB |= (1<<PB4);
	DDRB |= 0b00010111;
	DDRD &= ~(1<<PD2);
	DDRD &= ~(1<<PD4);
	char	i = 0;
	while (1){
		while (is_pushed(PD2)){
			if (!sw1_flag){
				if (i < 16)
					i++;
				// Actualiser les lights sur ++i
				actualise(i);
				sw1_flag = 1;
			}
		}
		
		while (is_pushed(PD4)){
			if (!sw2_flag){
				if (i > 0)
					i--;
				// Actualiser les lights sur --i
				actualise(i);
				sw2_flag = 1;
			}
		}
		sw1_flag = 0;
		sw2_flag = 0;
	}
	return (0);
}
