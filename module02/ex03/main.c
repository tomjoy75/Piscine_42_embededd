#include "main.h"

volatile char	c;

void	uart_init(void){
	int	baud_prescale = (uint16_t)(( F_CPU /16./BAUD) - 0.5);
	//UCSR0A |= (1 << U2X0);
	UBRR0H = (baud_prescale >> 8);
	UBRR0L = baud_prescale;
	UCSR0B = (1 << TXEN0) | (1 << RXEN0) | (1 << RXCIE0);  // Activate Tx and Rx and interrupt when a char is received
	UCSR0C = (1 << UCSZ01) | (1 << UCSZ00); // 8 bits mode
    //UCSR0B |= (1 << UDRIE0) | (1 << RXCIE0);  // Active l’interruption d’envoi UART et quand un caractere est recu
	sei();
}

void	uart_tx(void){
	while (!(UCSR0A & (1 << UDRE0))); // Checke that UDR0 is ready to receive a new char
	UDR0 = c;
}

ISR(USART_RX_vect){ // Interruption declenched by an input char(reception on RXD)
	c = UDR0; // I put the input in c ... It empty RXC0
	if (c >= 'a' && c <= 'z') // I transform alpha chars
		c -= 32;
	else if (c >= 'A' && c <= 'Z')
		c += 32;
	uart_tx();
}

int	main(void){
	uart_init();
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
| 💡 `UDR0` est utilisé pour lire un caractère reçu (`ISR(USART_RX_vect)`). |
| 💡 `UDR0` est aussi utilisé pour envoyer le caractère modifié (`uart_tx()`). |
--------------------------------------------------------------

--------------------------------------------------------------
|  UCSR0A – USART Control and Status Register A  (p.181 de la doc)  |
--------------------------------------------------------------
| RXC0  | TXC0  | UDRE0 | FE0   | DOR0  | UPE0  | U2X0  | MPCM0 |
|   7   |   6   |   5   |   4   |   3   |   2   |   1   |   0   |
--------------------------------------------------------------
| RXC0  | 1 = Un caractère a été reçu et est disponible dans `UDR0`. |
| TXC0  | 1 = Transmission terminée (dernier bit envoyé). |
| UDRE0 | 1 = `UDR0` est prêt à recevoir un nouveau caractère. |
| FE0   | 1 = Erreur de trame (Frame Error). |
| DOR0  | 1 = Erreur de dépassement du buffer (Data OverRun). |
| UPE0  | 1 = Erreur de parité (Parity Error). |
| U2X0  | 1 = Double la vitesse de transmission en mode asynchrone. |
| MPCM0 | 1 = Active le mode de communication multiprocesseur. |
--------------------------------------------------------------
|                                                             |
| 💡 `UDRE0` (bit 5) est utilisé dans `uart_tx()` pour s’assurer que `UDR0` est prêt à envoyer. |
| 💡 `RXC0` (bit 7) est géré automatiquement par l’interruption `ISR(USART_RX_vect)`. |
--------------------------------------------------------------

--------------------------------------------------------------
|  UCSR0B – USART Control and Status Register B  (p.182 de la doc)  |
--------------------------------------------------------------
| RXCIE0 | TXCIE0 | UDRIE0 | RXEN0  | TXEN0  | UCSZ02 | RXB80  | TXB80  |
|    7   |    6   |    5   |    4   |    3   |    2   |    1   |    0   |
--------------------------------------------------------------
| RXCIE0 | 1 = Active l’interruption quand un caractère est reçu (`ISR(USART_RX_vect)`). |
| TXCIE0 | 1 = Active l’interruption quand la transmission est terminée (`ISR(USART_TX_vect)`). |
| UDRIE0 | 1 = Active l’interruption quand `UDR0` est vide (`ISR(USART_UDRE_vect)`). |
| RXEN0  | 1 = Active la réception UART. |
| TXEN0  | 1 = Active la transmission UART. |
| UCSZ02 | Définit la taille des données envoyées (utilisé avec `UCSZ01:UCSZ00` dans `UCSR0C`). |
| RXB80  | Neuvième bit des données reçues (si on utilise 9 bits). |
| TXB80  | Neuvième bit des données transmises (si on utilise 9 bits). |
--------------------------------------------------------------
|                                                             |
| 💡 `TXEN0` (bit 3) est activé dans `uart_init()` pour permettre l'envoi (`uart_tx()`). |
| 💡 `RXEN0` (bit 4) est activé pour activer la réception (`ISR(USART_RX_vect)`). |
| 💡 `RXCIE0` (bit 7) est activé pour déclencher `ISR(USART_RX_vect)` lorsqu'un caractère est reçu. |
--------------------------------------------------------------

--------------------------------------------------------------
|  UCSR0C – USART Control and Status Register C  (p.183 de la doc)  |
--------------------------------------------------------------
| UMSEL01 | UMSEL00 | UPM01  | UPM00  | USBS0  | UCSZ01 | UCSZ00 | UCPOL0 |
|    7    |    6    |    5   |    4   |    3   |    2   |    1   |    0   |
--------------------------------------------------------------
| UMSEL01:UMSEL00 | 00 = Mode asynchrone, 01 = Mode synchrone, 10/11 = Mode SPI maître. |
| UPM01:UPM00     | 00 = Pas de parité, 10 = Parité paire, 11 = Parité impaire. |
| USBS0           | 0 = 1 bit de stop, 1 = 2 bits de stop. |
| UCSZ01:UCSZ00   | 00 = 5 bits, 01 = 6 bits, 10 = 7 bits, 11 = 8 bits (avec `UCSZ02` dans `UCSR0B`). |
| UCPOL0          | Change la polarité de l'horloge en mode synchrone. |
--------------------------------------------------------------
|                                                             |
| 💡 `UCSZ01:UCSZ00` (bits 2 et 1) sont activés dans `uart_init()` pour configurer le mode 8 bits. |
--------------------------------------------------------------

--------------------------------------------------------------
|  UBRR0H:UBRR0L – USART Baud Rate Register (p.184 de la doc)  |
--------------------------------------------------------------
| UBRR0H (4 bits) | UBRR0L (8 bits) |
--------------------------------------------------------------
| Définit le Baud Rate de l'USART. Calculé avec :
|     `UBRR0 = (F_CPU / (16 * Baud)) - 1`   (Mode normal). |
|     `UBRR0 = (F_CPU / (8 * Baud)) - 1`    (Si `U2X0 = 1` pour doubler la vitesse). |
--------------------------------------------------------------
|                                                             |
| 💡 `UBRR0H` et `UBRR0L` sont définis dans `uart_init()` pour fixer la vitesse de communication. |
--------------------------------------------------------------
*/

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
| TIMER1_COMPA             |  12    | Comparaison Timer1 (mode CTC)         | ❌ |
| TIMER1_COMPB             |  13    | Comparaison Timer1                    | ❌ |
| TIMER1_OVF               |  14    | Overflow Timer1                        | ❌ |
| TIMER0_COMPA             |  15    | Comparaison Timer0 (mode CTC)         | ❌ |
| TIMER0_COMPB             |  16    | Comparaison Timer0                    | ❌ |
| TIMER0_OVF               |  17    | Overflow Timer0                        | ❌ |
| SPI_STC                  |  18    | Fin de transmission SPI               | ❌ |
| USART_RX                 |  19    | Réception complète UART               | ✅ Utilisé |
| USART_UDRE               |  20    | Buffer de transmission UART vide      | ❌ |
| USART_TX                 |  21    | Transmission complète UART            | ❌ |
| ADC                      |  22    | Fin de conversion ADC                 | ❌ |
| EE_READY                 |  23    | EEPROM prête                          | ❌ |
| ANALOG_COMP              |  24    | Comparateur analogique                | ❌ |
| TWI                      |  25    | Interruption I2C (TWI)                | ❌ |
| SPM_READY                |  26    | Mémoire flash prête                   | ❌ |
--------------------------------------------------------------

--------------------------------------------------------------
|  USART_RX – Interruption quand un caractère est reçu  |
--------------------------------------------------------------
| ✅ Utilisé dans `ISR(USART_RX_vect)`. |
| Déclenché dès qu'un caractère est reçu et stocké dans `UDR0`. |
--------------------------------------------------------------
|                                                             |
| 💡 `ISR(USART_RX_vect)` récupère le caractère reçu depuis `UDR0`. |
| 💡 Modifie la casse du caractère (majuscule ↔ minuscule). |
| 💡 Appelle `uart_tx()` pour renvoyer le caractère transformé. |
--------------------------------------------------------------

--------------------------------------------------------------
|  USART_UDRE – Interruption quand `UDR0` est vide (Non utilisé)  |
--------------------------------------------------------------
| ❌ Non utilisé dans ce programme, mais pourrait être utile pour envoyer des caractères en interruption. |
--------------------------------------------------------------
|                                                             |
| 💡 Permettrait d’envoyer une chaîne de caractères sans bloquer `main()`. |
| 💡 Peut être activé avec `UCSR0B |= (1 << UDRIE0);` pour déclencher `ISR(USART_UDRE_vect)`. |
--------------------------------------------------------------
*/

