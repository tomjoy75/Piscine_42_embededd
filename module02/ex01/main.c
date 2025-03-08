#include "main.h"

char	uart_buffer[64];
int		uart_index = 0;

void	uart_init(void){
	int	baud_prescale = (int)( F_CPU / (16.0 * BAUD) - 0.5);
	UBRR0H = (baud_prescale >> 8);
	UBRR0L = baud_prescale;
	UCSR0B = (1 << TXEN0); // Activate Tx
	UCSR0C = (1 << UCSZ01) | (1 << UCSZ00); // 8 bits mode
}

void	timer_init(void){
	TCCR1B = (1<<WGM12); // CTC Mode
	OCR1A = 31247;	// (F_CPU / Prescaler * Temps) - 1	
	// (before prescale)F_CPU = 16Mhz = 16000000 Hz
	// (after prescale) F_CPU = 16000000 / 1024  = 15624
	// (1 operation by 2 sec) OCR1A = 15624 * 2 -1 = 31247
	TCCR1B |= (1<<CS12) | (1<<CS10);
	TIMSK1 |= (1<<OCIE1A);// Activate interruption Timer1 CTC
}

void uart_printstr(const char *str) {
	int i = 0;
	// Copie de la string dans le buffer
	while (str[i] && i < sizeof(uart_buffer) - 1){
		uart_buffer[i] = str[i];
		i++;
	}
	uart_buffer[i] = '\0';
    uart_index = 0;
    UCSR0B |= (1 << UDRIE0);  // Active l’interruption d’envoi UART
}

// ISR Timer1 → Appelle `uart_printstr()` toutes les 2 secondes
ISR(TIMER1_COMPA_vect) {
    uart_printstr("Hello World!\r\n");
}

// ISR UART → Envoie les caractères un par un
ISR(USART_UDRE_vect) {
	if (uart_buffer[uart_index]) { //TODO: Faut il checker si UDR0 est vide?
//		while (!(UCSR0A & (1 << UDRE0))); // Check that UDR0 is ready...Not necessary because already checked by Interruption
        UDR0 = uart_buffer[uart_index++];
    } else {
        UCSR0B &= ~(1 << UDRIE0);  // Désactive l'interruption une fois terminé
    }
}

int	main(void){
	uart_init();
	timer_init();
	sei();
	while(1){
	}
	return (0);
}

/*
--------------------------------------------------------------
|               USART – Universal Synchronous/Asynchronous Receiver/Transmitter               |
--------------------------------------------------------------

--------------------------------------------------------------
|  UDR0 – USART I/O Data Register  (p.185 de la doc)  |
--------------------------------------------------------------
|              UDR0[7:0] (8 bits de données)             |
--------------------------------------------------------------
| UDR0 | Contient le caractère en cours d’envoi ou reçu. |
--------------------------------------------------------------
|                                                             |
| 💡 `UDR0` est utilisé dans `ISR(USART_UDRE_vect)` pour envoyer un caractère. |
| 💡 `UDR0 = uart_buffer[uart_index++]` envoie le prochain caractère. |
--------------------------------------------------------------

--------------------------------------------------------------
|  UCSR0A – USART Control and Status Register A  (p.181 de la doc)  |
--------------------------------------------------------------
| RXC0  | TXC0  | UDRE0 | FE0   | DOR0  | UPE0  | U2X0  | MPCM0 |
|   7   |   6   |   5   |   4   |   3   |   2   |   1   |   0   |
--------------------------------------------------------------
| RXC0  | 1 = Un caractère a été reçu et est disponible dans UDR0
| TXC0  | 1 = Transmission terminée (dernier bit envoyé)
| UDRE0 | 1 = UDR0 est prêt à recevoir un nouveau caractère
| FE0   | 1 = Erreur de trame (Frame Error)
| DOR0  | 1 = Erreur de dépassement du buffer (Data OverRun)
| UPE0  | 1 = Erreur de parité (Parity Error)
| U2X0  | 1 = Double la vitesse de transmission en mode asynchrone
| MPCM0 | 1 = Active le mode de communication multiprocesseur
--------------------------------------------------------------
|                                                             |
| 💡 `UDRE0` (bit 5) est utilisé pour vérifier si `UDR0` est prêt. |
--------------------------------------------------------------


--------------------------------------------------------------
|  UCSR0B – USART Control and Status Register B  (p.182 de la doc)  |
--------------------------------------------------------------
| RXCIE0 | TXCIE0 | UDRIE0 | RXEN0  | TXEN0  | UCSZ02 | RXB80  | TXB80  |
|    7   |    6   |    5   |    4   |    3   |    2   |    1   |    0   |
--------------------------------------------------------------
| RXCIE0 | 1 = Active l’interruption quand un caractère est reçu (ISR(USART_RX_vect))
| TXCIE0 | 1 = Active l’interruption quand la transmission est terminée (ISR(USART_TX_vect))
| UDRIE0 | 1 = Active l’interruption quand UDR0 est vide (ISR(USART_UDRE_vect))
| RXEN0  | 1 = Active la réception UART
| TXEN0  | 1 = Active la transmission UART (sur TXD(PD1) bit par bit)
| UCSZ02 | Définit la taille des données envoyées (utilisé avec UCSZ01:UCSZ00 dans UCSR0C)
| RXB80  | Neuvième bit des données reçues (si on utilise 9 bits)
| TXB80  | Neuvième bit des données transmises (si on utilise 9 bits)
--------------------------------------------------------------
| 💡 `TXEN0` (bit 3) est utilisé dans `uart_init()` pour activer l’émetteur UART. |
| 💡 `UDRIE0` (bit 5) est activé dans `uart_printstr()` pour déclencher `ISR(USART_UDRE_vect)`. |
| 💡 `UDRIE0` est désactivé dans `ISR(USART_UDRE_vect)` une fois l’envoi terminé. |
--------------------------------------------------------------

--------------------------------------------------------------
|  UCSR0C – USART Control and Status Register C  (p.183 de la doc)  |
--------------------------------------------------------------
| UMSEL01 | UMSEL00 | UPM01  | UPM00  | USBS0  | UCSZ01 | UCSZ00 | UCPOL0 |
|    7    |    6    |    5   |    4   |    3   |    2   |    1   |    0   |
--------------------------------------------------------------
| UMSEL01:UMSEL00 | 00 = Mode asynchrone, 01 = Mode synchrone, 10/11 = Mode SPI maître
| UPM01:UPM00     | 00 = Pas de parité, 10 = Parité paire, 11 = Parité impaire
| USBS0           | 0 = 1 bit de stop, 1 = 2 bits de stop
| UCSZ01:UCSZ00   | 00 = 5 bits, 01 = 6 bits, 10 = 7 bits, 11 = 8 bits (avec UCSZ02 dans UCSR0B)
| UCPOL0          | Change la polarité de l'horloge en mode synchrone
--------------------------------------------------------------
| UCSZ01:UCSZ00 | 11 = Mode 8 bits (✅ Utilisé dans `uart_init()`) |
| ⚠️ Pas de parité, pas de mode synchrone. |

--------------------------------------------------------------
--------------------------------------------------------------
|  UBRR0H:UBRR0L – USART Baud Rate Register (p.184 de la doc)  |
--------------------------------------------------------------
| UBRR0H (4 bits) | UBRR0L (8 bits) |
--------------------------------------------------------------
| Définit le Baud Rate de l'USART. Calculé avec :
|     UBRR0 = (F_CPU / (16 * Baud)) - 1   (Mode normal)
|     UBRR0 = (F_CPU / (8 * Baud)) - 1    (Si U2X0 = 1 pour doubler la vitesse)
*/
/*
/*
--------------------------------------------------------------
|               INTERRUPTIONS – ATmega328P (p.58 de la doc)               |
--------------------------------------------------------------

--------------------------------------------------------------
|  Vecteur d'interruption  | Numéro | Description                           | Utilisé ? |
--------------------------------------------------------------
| RESET                    |   1    | Réinitialisation du microcontrôleur   | ❌ |
| INT0                     |   2    | Interruption externe sur PD2          | ❌ |
| INT1                     |   3    | Interruption externe sur PD3          | ❌ |
| PCINT0                   |   4    | Interruption changement de pin (D8-D13) | ❌ |
| PCINT1                   |   5    | Interruption changement de pin (A0-A5) | ❌ |
| PCINT2                   |   6    | Interruption changement de pin (D0-D7) | ❌ |
| WDT                      |   7    | Interruption Watchdog Timer           | ❌ |
| TIMER2_COMPA             |   8    | Comparaison Timer2 (mode CTC)         | ❌ |
| TIMER2_COMPB             |   9    | Comparaison Timer2                    | ❌ |
| TIMER2_OVF               |  10    | Overflow Timer2                        | ❌ |
| TIMER1_CAPT              |  11    | Capture d'entrée Timer1               | ❌ |
| TIMER1_COMPA             |  12    | Comparaison Timer1 (mode CTC)         | ✅ Utilisé |
| TIMER1_COMPB             |  13    | Comparaison Timer1                    | ❌ |
| TIMER1_OVF               |  14    | Overflow Timer1                        | ❌ |
| TIMER0_COMPA             |  15    | Comparaison Timer0 (mode CTC)         | ❌ |
| TIMER0_COMPB             |  16    | Comparaison Timer0                    | ❌ |
| TIMER0_OVF               |  17    | Overflow Timer0                        | ❌ |
| SPI_STC                  |  18    | Fin de transmission SPI               | ❌ |
| USART_RX                 |  19    | Réception complète UART               | ❌ (non utilisé mais utile) |
| USART_UDRE               |  20    | Buffer de transmission UART vide      | ✅ Utilisé |
| USART_TX                 |  21    | Transmission complète UART            | ❌ |
| ADC                      |  22    | Fin de conversion ADC                 | ❌ |
| EE_READY                 |  23    | EEPROM prête                          | ❌ |
| ANALOG_COMP              |  24    | Comparateur analogique                | ❌ |
| TWI                      |  25    | Interruption I2C (TWI)                | ❌ |
| SPM_READY                |  26    | Mémoire flash prête                   | ❌ |
--------------------------------------------------------------

--------------------------------------------------------------
|  TIMER1_COMPA – Interruption Timer1 en mode CTC  |
--------------------------------------------------------------
| ✅ Utilisé dans `ISR(TIMER1_COMPA_vect)`. |
| Déclenché toutes les 2 secondes grâce à `OCR1A`. |
--------------------------------------------------------------
|                                                             |
| 💡 `ISR(TIMER1_COMPA_vect)` appelle `uart_printstr("Hello World!\r\n")`. |
| 💡 Cette interruption permet d'envoyer la chaîne sans bloquer `main()`. |
--------------------------------------------------------------

--------------------------------------------------------------
|  USART_UDRE – Interruption quand `UDR0` est vide  |
--------------------------------------------------------------
| ✅ Utilisé dans `ISR(USART_UDRE_vect)`. |
| Déclenché quand `UDRE0 = 1`, indiquant que `UDR0` est prêt pour un nouveau caractère. |
--------------------------------------------------------------
|                                                             |
| 💡 `ISR(USART_UDRE_vect)` envoie les caractères du `uart_buffer` un par un. |
| 💡 Si la transmission est terminée, `UDRIE0` est désactivé. |
--------------------------------------------------------------

--------------------------------------------------------------
|  USART_RX – Interruption quand un caractère est reçu (Non utilisé)  |
--------------------------------------------------------------
| ❌ Non utilisé dans ce programme, mais pourrait être utile pour la réception UART. |
--------------------------------------------------------------
|                                                             |
| 💡 Permet de recevoir les caractères sans bloquer `main()`. |
| 💡 Peut être activé en mettant `UCSR0B |= (1 << RXCIE0);`. |
--------------------------------------------------------------
*/
