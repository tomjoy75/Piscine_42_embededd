#include "main.h"
/*
volatile int direction = 1; // 1 or -1
volatile int percent = 0;

void	timer1_init(void){
	DDRB |= (1<<D2); // Configure PB1 as output(LED D2)	
	TCCR1A |= (1<<WGM11); // Fast PWM
	TCCR1B |= (1<<WGM12) | (1<<WGM13); 
	TCCR1A |= (1<<COM1A1); // Mode clear OC1A on compare match 
	   // OC1A passe LOW quand TCNT1 atteint OCR1A
    TCCR1B |= (1 << CS11);  // Prescaler 8
	ICR1 = 19999;// (F_CPU / Prescaler * Temps) - 1
	// Pour avoir 100 Hz => 16 MHz / (8 * 20000) - 1
	OCR1A = 0;
}

void	timer0_init(void){
	TCCR0A |= (1<<WGM01); // CTC
	TCCR0B |= (1<<CS00) | (1<<CS02); // prescale at /1024
	OCR0A = 77; // F_CPU / (Prescaler * f_interruption) - 1 avec f_interruption a 100Hz
	// 200 incrementations/decrementations par sec => 200 Hz
	// 16MHz / (1024 * 200) - 1
	TIMSK0 |= (1 << OCIE0A); // Active l'interruption sur OCR0A

}

// ISR(TIMER0_COMPA_vect){
// 	OCR1A = (uint16_t)percent * (ICR1 / 100);
// 	if (ascend)//TODO: Transformer en operateur ternaire
// 		percent++;
// 	else
// 		percent--;
// 	if (ascend && percent >= 100)
// 		ascend = 0;
// 	if (!ascend && percent <= 0)
// 		ascend = 1;
// }

ISR(TIMER0_COMPA_vect){
	percent += direction;

	if (percent == 100)
		direction = -1;
	else if (percent == 0)
		direction = 1;
 	OCR1A = percent * (ICR1 / 100);
}

int	main(void){
	timer1_init();
	timer0_init();
	sei();
	while (1)
	{
	}
	return(0);
}

void timer1_init(void){
	TCCR1B |= (1<<WGM12); // CTC
	TCCR1B |= (1<<CS10) | (1<<CS12); // prescale at /1024
	OCR1A = 77; // F_CPU / (Prescaler * f_interruption) - 1 avec f_interruption a 100Hz
	// 200 incrementations/decrementations par sec => 200 Hz
	// 16MHz / (1024 * 200) - 1
	TIMSK1 |= (1 << OCIE1A); // Active l'interruption sur OCR0A

}

void sw_init(void){
	
}
*/
////////////////
volatile char i = 0;

// int	is_pushed(char pin){

// 	if ((PIND & (1<<pin)) == 0)
// 	{
// 		_delay_ms(BOUNCE_DELAY); // Protection for the bounce
// 		if ((PIND & (1<<pin)) == 0)
// 			return (1);
// 	}
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

ISR(INT0_vect){
	// EIMSK &= ~(1<<INT0); /// Gestion du debounce
	// _delay_ms(275);
	// EIMSK |= (1<<INT0); 
	if (i < 15)
		i++;
	actualise(i);
}

ISR(INT1_vect){
//	PORTB ^= (1<<D1);  // Toggle LED D1 pour voir si l'ISR fonctionne
	// EIMSK &= ~(1<<INT1); /// Gestion du debounce
	// _delay_ms(275);
	// EIMSK |= (1<<INT1); 
	//	PORTB ^= (1<<PB0); // Inverse l’état de la LED sur PB0
	if (i > 0)
	i--;
	actualise(i);
}

int	main(void){

//	char sw1_flag = 0;
//	char sw2_flag = 0;
	led_init();
//	EICRA |= (1<<ISC01) | (1<<ISC11); // Mode de declencheent des interruptions
	EICRA |= (1<<ISC01) | (1<<ISC00) | (1<<ISC11) | (1<<ISC10);

	EIMSK |= (1<<INT0) | (1<<INT1);
//	DDRD &= ~(1<<PD2) | ~(1<<PD4);
	PORTD |= (1<<PD2) | (1<<PD4);
	// DDRB |= (1<<PB0);
	// DDRB |= (1<<PB1);
	// DDRB |= (1<<PB2);
	// DDRB |= (1<<PB4);
	//	DDRB |= 0b00010111;
	DDRD &= ~(1<<PD2);
	DDRD &= ~(1<<PD4);
	sei();
	while (1){
		// while (is_pushed(PD2)){
		// 	if (!sw1_flag){
		// 		if (i < 15)
		// 			i++;
		// 		// Actualiser les lights sur ++i
		// 		actualise(i);
		// 		sw1_flag = 1;
		// 	}
		// }
		
		// while (is_pushed(PD4)){
		// 	if (!sw2_flag){
		// 		if (i > 0)
		// 			i--;
		// 		// Actualiser les lights sur --i
		// 		actualise(i);
		// 		sw2_flag = 1;
		// 	}
		// }
		// sw1_flag = 0;
		// sw2_flag = 0;
	}
	return (0);
}

/*
--------------------------------------------------------------
|               INTERRUPTIONS – ATmega328P (p.58 de la doc)               |
--------------------------------------------------------------

--------------------------------------------------------------
|  Vecteur d'interruption  | Numéro | Description                           | Utilisé ? |
--------------------------------------------------------------
| RESET                    |   1    | Réinitialisation du microcontrôleur   | ❌ |
| INT0                     |   2    | Interruption externe sur PD2          | ❌  |
| INT1                     |   3    | Interruption externe sur PD3          | ❌ |
| PCINT0                   |   4    | Interruption changement de pin (D8-D13) | ❌ |
| PCINT1                   |   5    | Interruption changement de pin (A0-A5) | ❌ |
| PCINT2                   |   6    | Interruption changement de pin (D0-D7) | ❌ |
| WDT                      |   7    | Interruption Watchdog Timer           | ❌ |
| TIMER2_COMPA             |   8    | Comparaison Timer2 (mode CTC)         | ❌ |
| TIMER2_COMPB             |   9    | Comparaison Timer2                    | ❌ |
| TIMER2_OVF               |  10    | Overflow Timer2                        | ❌ |
| TIMER1_CAPT              |  11    | Capture d'entrée Timer1               | ❌ |
| TIMER1_COMPA             |  12    | Comparaison Timer1 (mode CTC)         |  ❌ |
| TIMER1_COMPB             |  13    | Comparaison Timer1                    | ❌ |
| TIMER1_OVF               |  14    | Overflow Timer1                        | ❌ |
| TIMER0_COMPA             |  15    | Comparaison Timer0 (mode CTC)         | ✅ Utilisé |
| TIMER0_COMPB             |  16    | Comparaison Timer0                    | ❌ |
| TIMER0_OVF               |  17    | Overflow Timer0                        | ❌ |
| SPI_STC                  |  18    | Fin de transmission SPI               | ❌ |
| USART_RX                 |  19    | Réception complète UART               | ❌|
| USART_UDRE               |  20    | Buffer de transmission UART vide      | ❌ |
| USART_TX                 |  21    | Transmission complète UART            | ❌ |
| ADC                      |  22    | Fin de conversion ADC                 | ❌ |
| EE_READY                 |  23    | EEPROM prête                          | ❌ |
| ANALOG_COMP              |  24    | Comparateur analogique                | ❌ |
| TWI                      |  25    | Interruption I2C (TWI)                | ❌ |
| SPM_READY                |  26    | Mémoire flash prête                   | ❌ |
--------------------------------------------------------------
*/
/*
--------------------------------------------------------------
|               INTERRUPTIONS – Registres ATmega328P               |
--------------------------------------------------------------

--------------------------------------------------------------
|  EIMSK – External Interrupt Mask Register  (p.74 de la doc)  |
--------------------------------------------------------------
|  -  |  -  |  -  |  -  |  -  |  -  |  -  | INT0  |
--------------------------------------------------------------
| INT0 | Active l’interruption externe `INT0` sur PD2. |
--------------------------------------------------------------
|                                                             |
| 💡 `INT0` (bit 0) est activé pour détecter un changement sur `PD2`. |
--------------------------------------------------------------

--------------------------------------------------------------
|  EICRA – External Interrupt Control Register A  (p.75 de la doc)  |
--------------------------------------------------------------
|  -  |  -  |  -  |  -  |  -  |  -  | ISC01  | ISC00  |
--------------------------------------------------------------
| ISC01:ISC00 | Définit le mode de déclenchement de `INT0`. |
| 00 = Niveau bas, 01 = Front montant, 10 = Front descendant, 11 = Flanc montant. |
--------------------------------------------------------------
|                                                             |
| 💡 `ISC01` = 1, `ISC00` = 0 → Déclenche `INT0` sur **front descendant**. |
--------------------------------------------------------------

--------------------------------------------------------------
|  EIFR – External Interrupt Flag Register  (p.76 de la doc)  |
--------------------------------------------------------------
|  -  |  -  |  -  |  -  |  -  |  -  |  -  | INTF0  |
--------------------------------------------------------------
| INTF0 | 1 = Indique qu’une interruption `INT0` s'est produite. |
--------------------------------------------------------------
|                                                             |
| 💡 `INTF0` doit être effacé en écrivant `1` dessus après traitement. |
--------------------------------------------------------------

--------------------------------------------------------------
|  TIMSK1 – Timer Interrupt Mask Register  (p.136 de la doc)  |
--------------------------------------------------------------
|  -  |  -  |  -  |  -  |  -  |  -  | OCIE1A |  -  |
--------------------------------------------------------------
| OCIE1A | 1 = Active l’interruption de comparaison Timer1 (CTC mode). |
--------------------------------------------------------------
|                                                             |
| 💡 `OCIE1A` est activé pour déclencher `ISR(TIMER1_COMPA_vect)`. |
--------------------------------------------------------------

--------------------------------------------------------------
|  TCCR1B – Timer/Counter1 Control Register B  (p.134 de la doc)  |
--------------------------------------------------------------
|  -  |  -  |  -  |  -  | WGM12 |  -  | CS12  | CS10  |
--------------------------------------------------------------
| WGM12 | 1 = Active le mode CTC (Clear Timer on Compare Match). |
| CS12:CS10 | 10 = Prescaler 1024. |
--------------------------------------------------------------
|                                                             |
| 💡 `WGM12` active le mode CTC pour générer une interruption périodique. |
| 💡 `CS12 = 1, CS10 = 1` définit un **prescaler de 1024**. |
--------------------------------------------------------------

--------------------------------------------------------------
|  UCSR0B – USART Control and Status Register B  (p.182 de la doc)  |
--------------------------------------------------------------
| RXCIE0 | TXCIE0 | UDRIE0 | RXEN0  | TXEN0  | UCSZ02 | RXB80  | TXB80  |
--------------------------------------------------------------
| RXCIE0 | 1 = Active l’interruption à la réception d’un caractère (`ISR(USART_RX_vect)`). |
| TXCIE0 | 1 = Active l’interruption quand la transmission est terminée (`ISR(USART_TX_vect)`). |
| UDRIE0 | 1 = Active l’interruption quand `UDR0` est vide (`ISR(USART_UDRE_vect)`). |
| RXEN0  | 1 = Active la réception UART. |
| TXEN0  | 1 = Active la transmission UART. |
--------------------------------------------------------------
|                                                             |
| 💡 `TXEN0` est activé pour permettre l’envoi (`uart_tx()`). |
| 💡 `UDRIE0` est activé pour envoyer les caractères un par un via `ISR(USART_UDRE_vect)`. |
--------------------------------------------------------------
*/
