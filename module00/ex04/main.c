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

int	is_lighted(char port){
	if ((PINB & (1<<port)) == 1)
	       return (1);
	return (0);	
}

void	actualise(char i){
	PORTB &= ~(0b00010111);  // Effacer PB0 à PB3
	PORTB |= (i & 0b00010111);  // Mettre à jour les LEDs
}

int	main(void){

	char sw1_flag = 0;
	char sw2_flag = 0;
	DDRB |= (1<<PB0);
	DDRB |= (1<<PB1);
	DDRB |= (1<<PB2);
	DDRB |= (1<<PB4);
	DDRD &= ~(1<<PD2);
	DDRD &= ~(1<<PD4);
	char	i = 0;
	while (1){
		while (is_pushed(PD2)){
			if (!sw1_flag){
				// Actualiser les lights sur ++i
				actualise(++i);
				sw1_flag = 1;
			}
		}
		
		while (is_pushed(PD4)){
			if (!sw2_flag){
				// Actualiser les lights sur --i
				actualise(--i);
				sw2_flag = 1;
			}
		}
		sw1_flag = 0;
		sw2_flag = 0;
	}
	return (0);
}
