#include "main.h"
/*
// ISR (Interrupt Service Routine) exécutée quand INT0 est déclenché
ISR(INT0_vect){
	EIMSK &= ~(1<<INT0); /// Gestion du debounce
	_delay_ms(50);
	EIMSK |= (1<<INT0); 
	PORTB ^= (1<<PB0); // Inverse l’état de la LED sur PB0
}

int	main(void){
	led_init(); // Initialise les LED sur PB0 (et les autres)
	EICRA |= (1<<ISC01); // Mode de declenchement de l'interruption (se declenche quand on passe de high a low)
	EIMSK |= (1<<INT0); // Active l’interruption externe INT0
	DDRD &= ~(1<<PD2); // Met PD2 en entree
	PORTD |= (1<<PD2); // opt. resistance pull up
	sei(); // Active bit I de SREG (Status Regster) qui active les interruptions
	while (1)
	{
	}
	return (0);
}*/
uint8_t ascend = 1;
uint8_t percent = 0;

ISR(TIMER0_COMPA_vect){
	PORTB |= (1<<PB0); // Debug
	OCR1A = percent * ICR1 / 100;
	if (ascend)//TODO: Transformer en operateur ternaire
		percent++;
	else
		percent--;
	if (ascend && percent == 100)
		ascend = 0;
	if (!ascend && percent == 0)
		ascend = 1;
}

int	main(void){
	DDRB |= (1<<PB1); // Configure PB1 as output(LED D2)	
	DDRB |= (1<<PB0); // Debug
	// Gestion Timer1
	TCCR1A |= (1<<COM1A1); // 
	TCCR1A |= (1<<WGM10); // Fast PWM
	TCCR1B |= (1<<WGM12); 
	ICR1 = 7811;
	OCR1A = 781; // (F_CPU / Prescaler * Temps) - 1
	// (before prescale)F_CPU = 16Mhz = 16000000 Hz
	// (after prescale) F_CPU = 16000000 / 1024  = 15624
	// (2 operation by sec) OCR1A = 15624 / 2 -1 = 7811
//	TCCR1A |= (1<<COM1A0); // Activate toggle OC1A to invert automatically PB1  
	TCCR1B |= (1<<CS12) | (1<<CS10); // prescale at /1024 and launch the timer

	// Gestion Timer0
	TCCR0A |= (1<<WGM01); // CTC
	TCCR0A |= (1<<COM0A0); // Toggle OC0A on compare match
	TCCR0B |= (1<<CS00) | (1<<CS02); // prescale at /1024
	OCR0A = 78;
	sei();
	while (1)
	{
		// OCR1A++;
		// _delay_ms(20);
	}

	return(0);
}
/*
--------------------------------------------------------------
|               INTERRUPTIONS – ATmega328P (p.58 de la doc)               |
--------------------------------------------------------------

--------------------------------------------------------------
|  Vecteur d'interruption  | Numéro | Description                           | Utilisé ? |
--------------------------------------------------------------
| RESET                    |   1    | Réinitialisation du microcontrôleur   | ❌ |
| INT0                     |   2    | Interruption externe sur PD2          | ✅ Utilisé  |
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
| TIMER0_COMPA             |  15    | Comparaison Timer0 (mode CTC)         | ❌ |
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
