#include "main.h"

volatile uint8_t	state = 0;
char		*color_hexa;

uint8_t	check_hexa(const char *str){
	uint8_t i = 0;
	for (; i < 6; i++){
		if (str[i] < '0' || (str[i] > '9' && str[i] < 'A') || str[i] > 'F'){
			uart_printstr("not a valid code...Try again\r\033[1B");
			return (0);
		}
	}
	if (str[i]){
		uart_printstr("not a valid code...Try again\r\033[1B");
		return(0);
	}
	return (1);
}

uint8_t convert_hexa(char msd, char lsd){
	// msd/lsd: most/least significant digit
	uint8_t answer = 0;
	if (msd >= '0' && msd <= '9')
		answer += (msd - '0') * 16;
	else if (msd >= 'A' && msd <= 'F')
		answer += (msd - 'A' + 10) * 16;
	if (lsd >= '0' && lsd <= '9')
		answer += (lsd - '0');
	else if (lsd >= 'A' && lsd <= 'F')
		answer += (lsd - 'A' + 10);
	return (answer);	
}

int	main(void){
	uart_init();
	init_rgb();
	while(1){
		do {
			uart_printstr("Enter color code(hexa format) : #");
			color_hexa = uart_readstr();
		} while (!check_hexa(color_hexa));
		set_rgb(convert_hexa(color_hexa[0], color_hexa[1]), convert_hexa(color_hexa[2], color_hexa[3]), convert_hexa(color_hexa[4], color_hexa[5]));
		uart_printstr("\r\033[1B");
	}
	return (0);
}

/*
--------------------------------------------------------------
|            TIMER0 – Registres pour PWM (Rouge & Vert)      |
--------------------------------------------------------------

--------------------------------------------------------------
|  TCCR0A – Timer/Counter Control Register A (p.108 de la doc)  |
--------------------------------------------------------------
| COM0A1 | COM0A0 | COM0B1 | COM0B0 |  -  |  -  | WGM01 | WGM00 |
|   7    |   6    |   5    |   4    |  3  |  2  |   1   |   0   |
--------------------------------------------------------------
| COM0A1 | 1 = Active le PWM sur OC0A (✅ LED Rouge sur PD6) |
| COM0B1 | 1 = Active le PWM sur OC0B (✅ LED Verte sur PD5) |
| WGM01:WGM00 | 11 = Mode **Fast PWM** (comptage de 0 à 255) |
--------------------------------------------------------------
|                                                             |
| 💡 Utilisé pour générer un signal PWM sur Rouge et Vert.   |
--------------------------------------------------------------

--------------------------------------------------------------
|  TCCR0B – Timer/Counter Control Register B (p.109 de la doc)  |
--------------------------------------------------------------
|  -  |  -  |  -  |  -  | WGM02 | CS02 | CS01 | CS00 |
|  7  |  6  |  5  |  4  |   3   |   2  |   1  |   0  |
--------------------------------------------------------------
| CS01 | 1 = Prescaler **8** → Fréquence PWM ≈ 7.81 kHz |
--------------------------------------------------------------
|                                                             |
| 💡 Permet d’avoir une fréquence PWM idéale pour LED RGB.  |
--------------------------------------------------------------

--------------------------------------------------------------
|            TIMER2 – Registres pour PWM (Bleu)              |
--------------------------------------------------------------

--------------------------------------------------------------
|  TCCR2A – Timer/Counter Control Register A (p.128 de la doc)  |
--------------------------------------------------------------
| COM2A1 | COM2A0 | COM2B1 | COM2B0 |  -  |  -  | WGM21 | WGM20 |
|   7    |   6    |   5    |   4    |  3  |  2  |   1   |   0   |
--------------------------------------------------------------
| COM2B1 | 1 = Active le PWM sur OC2B (✅ LED Bleue sur PD3) |
| WGM21:WGM20 | 11 = Mode **Fast PWM** (comptage de 0 à 255) |
--------------------------------------------------------------
|                                                             |
| 💡 Utilisé pour générer un signal PWM sur la LED Bleue.   |
--------------------------------------------------------------

--------------------------------------------------------------
|  TCCR2B – Timer/Counter Control Register B (p.129 de la doc)  |
--------------------------------------------------------------
| FOC2A | FOC2B |  -  |  -  | WGM22 | CS22 | CS21 | CS20 |
|   7   |   6   |  5  |  4  |   3   |   2  |   1  |   0  |
--------------------------------------------------------------
| CS21 | 1 = Prescaler **8** → Fréquence PWM ≈ 7.81 kHz |
--------------------------------------------------------------
|                                                             |
| 💡 Permet d’avoir une fréquence PWM idéale pour LED RGB.  |
--------------------------------------------------------------

--------------------------------------------------------------
|  OCR0A, OCR0B, OCR2B – Output Compare Registers (p.112, 131)  |
--------------------------------------------------------------
| OCR0A (8 bits) = Intensité du Rouge (0-255) |
| OCR0B (8 bits) = Intensité du Vert (0-255)  |
| OCR2B (8 bits) = Intensité du Bleu (0-255)  |
--------------------------------------------------------------
|                                                             |
| 💡 Plus la valeur est élevée, plus la LED est lumineuse. |
--------------------------------------------------------------

*/
