#include "main.h"

void	uart_init(void){
	int	baud_prescale = (int)( F_CPU / (16.0 * BAUD) - 0.5);
	UBRR0H = (baud_prescale >> 8);
	UBRR0L = baud_prescale;
	UCSR0B = (1 << TXEN0) | (1 << RXEN0) ;  // Activate Tx and Rx
	UCSR0C = (1 << UCSZ01) | (1 << UCSZ00); // 8 bits mode
}

void	uart_tx(char c){
	while (!(UCSR0A & (1 << UDRE0))); // Wait for UDR0 to be ready to receive another char
	UDR0 = c;
}

char	uart_rx( void ){
	while (!(UCSR0A & (1 << RXC0))); // Wait for reception of a char 
	return (UDR0);
}

int	main(void){
	uart_init();
	while(1){
		uart_tx(uart_rx());
//		_delay_ms(DELAY);
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
| 💡 `UDR0` est utilisé pour stocker et envoyer les caractères via `uart_tx()`. |
| 💡 `UDR0` est lu dans `uart_rx()` pour récupérer un caractère reçu. |
--------------------------------------------------------------

--------------------------------------------------------------
|  UCSR0A – USART Control and Status Register A  (p.181 de la doc)  |
--------------------------------------------------------------
| RXC0  | TXC0  | UDRE0 | FE0   | DOR0  | UPE0  | U2X0  | MPCM0 |
|   7   |   6   |   5   |   4   |   3   |   2   |   1   |   0   |
--------------------------------------------------------------
| RXC0  | 1 = Un caractère a été reçu et est disponible dans UDR0. |
| TXC0  | 1 = Transmission terminée (dernier bit envoyé). |
| UDRE0 | 1 = UDR0 est prêt à recevoir un nouveau caractère. |
| FE0   | 1 = Erreur de trame (Frame Error). |
| DOR0  | 1 = Erreur de dépassement du buffer (Data OverRun). |
| UPE0  | 1 = Erreur de parité (Parity Error). |
| U2X0  | 1 = Double la vitesse de transmission en mode asynchrone. |
| MPCM0 | 1 = Active le mode de communication multiprocesseur. |
--------------------------------------------------------------
|                                                             |
| 💡 `RXC0` (bit 7) est utilisé dans `uart_rx()` pour vérifier si un caractère est disponible. |
| 💡 `while (!(UCSR0A & (1 << RXC0)));` attend qu’un caractère soit reçu avant de le lire. |
| 💡 `while (!(UCSR0A & (1 << UDRE0)));` attend qu’UDR0 soit prêt à recevoir un nouveau caractère.
--------------------------------------------------------------

--------------------------------------------------------------
|  UCSR0B – USART Control and Status Register B  (p.182 de la doc)  |
--------------------------------------------------------------
| RXCIE0 | TXCIE0 | UDRIE0 | RXEN0  | TXEN0  | UCSZ02 | RXB80  | TXB80  |
|    7   |    6   |    5   |    4   |    3   |    2   |    1   |    0   |
--------------------------------------------------------------
| RXCIE0 | 1 = Active l’interruption quand un caractère est reçu (ISR(USART_RX_vect)). |
| TXCIE0 | 1 = Active l’interruption quand la transmission est terminée (ISR(USART_TX_vect)). |
| UDRIE0 | 1 = Active l’interruption quand UDR0 est vide (ISR(USART_UDRE_vect)). |
| RXEN0  | 1 = Active la réception UART. |
| TXEN0  | 1 = Active la transmission UART. |
| UCSZ02 | Définit la taille des données envoyées (utilisé avec UCSZ01:UCSZ00 dans UCSR0C). |
| RXB80  | Neuvième bit des données reçues (si on utilise 9 bits). |
| TXB80  | Neuvième bit des données transmises (si on utilise 9 bits). |
--------------------------------------------------------------
|                                                             |
| 💡 `TXEN0` (bit 3) est activé dans `uart_init()` pour permettre l'envoi de caractères (`uart_tx()`). |
| 💡 `RXEN0` (bit 4) est activé dans `uart_init()` pour activer la réception (`uart_rx()`). |
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
| UCSZ01:UCSZ00   | 00 = 5 bits, 01 = 6 bits, 10 = 7 bits, 11 = 8 bits (avec UCSZ02 dans UCSR0B). |
| UCPOL0          | Change la polarité de l'horloge en mode synchrone. |
--------------------------------------------------------------
|                                                             |
| 💡 `UCSZ01:UCSZ00` (bits 2 et 1) sont utilisés dans `uart_init()` pour configurer le mode 8 bits. |
--------------------------------------------------------------

--------------------------------------------------------------
|  UBRR0H:UBRR0L – USART Baud Rate Register (p.184 de la doc)  |
--------------------------------------------------------------
| UBRR0H (4 bits) | UBRR0L (8 bits) |
--------------------------------------------------------------
| Définit le Baud Rate de l'USART. Calculé avec :
|     UBRR0 = (F_CPU / (16 * Baud)) - 1   (Mode normal). |
|     UBRR0 = (F_CPU / (8 * Baud)) - 1    (Si U2X0 = 1 pour doubler la vitesse). |
--------------------------------------------------------------
|                                                             |
| 💡 `UBRR0H` et `UBRR0L` sont définis dans `uart_init()` pour fixer la vitesse de communication. |
--------------------------------------------------------------
*/

