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

int	main(void){

	char pushed_flag = 0;
	DDRB |= (1<<PB0);
	DDRD &= ~(1<<PD2);
	while (1){
		while (is_pushed(PD2)){
			if (!pushed_flag){
				if (is_lighted(PB0))
					PORTB &= ~(1<<PB0);
				else
					PORTB |= (1<<PB0);
				pushed_flag = 1;
			}
		}
		pushed_flag = 0;
	}
	return (0);
}

