#include "main.h"

volatile uint16_t debounce_counter = 0; // Compteur du debounce
volatile uint8_t debounce_active = 0; // Indique si le debounce est actif

void timer0_init(void) {
	TCCR0A |= (1 << WGM01); // Mode CTC
    TCCR0B |= (1 << CS02) | (1 << CS00); // Prescaler 1024
    OCR0A = 15; // F_CPU / (1024 * 1000) - 1 = 250 (1ms par tick)
    TIMSK0 |= (1 << OCIE0A); // Active l'interruption sur OCR0A
}

/*INTERRUPTION TIMER0_COMPA_vect():
    Si le debounce est actif:
        Incrémenter le compteur de debounce
        Si debounce_counter ≥ 275 ms:
            Désactiver le debounce
            Réactiver INT0 et PCINT2*/
ISR(TIMER0_COMPA_vect){
	if (debounce_active) {
		debounce_counter++;
		if (debounce_counter >= BOUNCE_DELAY){ // En ms
			debounce_active = 0; // Desactive debounce
			EIMSK |= (1<<INT0); // Reactive INT0
//			PCICR |= (1 << PCIE2);  // Reactive PCINT2
		}
	}
}

// ISR (Interrupt Service Routine) exécutée quand INT0 est déclenché
ISR(INT0_vect){
	if (!debounce_active){
		debounce_active = 1; // Active le flag de debounce
		debounce_counter = 0; // Reinitialise le compteur
		EIMSK &= ~(1<<INT0); /// Gestion du debounce
//		_delay_ms(10);
//		EIMSK |= (1<<INT0); 
		if (!(PIND & (1 << PD2))) // Vérifie si le bouton est TOUJOURS pressé après 10ms
			PORTB ^= (1<<PB0); // Inverse l’état de la LED sur PB0
	}
}

int	main(void){
	led_init(); // Initialise les LED sur PB0 (et les autres)
	timer0_init();
	EICRA |= (1<<ISC01); // Mode de declenchement de l'interruption (se declenche quand on passe de high a low)
	EIMSK |= (1<<INT0); // Active l’interruption externe INT0
	DDRD &= ~(1<<PD2); // Met PD2 en entree
	PORTD |= (1<<PD2); // opt. resistance pull up
	sei(); // Active bit I de SREG (Status Regster) qui active les interruptions
	while (1)
	{
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
