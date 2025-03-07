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

// void uart_printstr(const char* str){
// 	//strncpy(uart_buffer, str, 12);
// 	uart_index = 0;
// 	UCSR0B |= (1 << UDRIE0);  // Activate interruption
// 	while (str[uart_index]){
// 		while (!(UCSR0A & (1<<UDRE0)));
// 		UDR0 = str[uart_index++];
// 	}
// 	while (!(UCSR0A & (1<<UDRE0))); // TODO: Comment ca fonctionne?
// 	UDR0 = '\r';
// 	while (!(UCSR0A & (1<<UDRE0)));
// 	UDR0 = '\n';
// } 

// ISR(TIMER1_COMPA_vect){
// 	uart_printstr("Hello world!");
// }

void uart_printstr(const char *str) {
	int i = 0;
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
//	PORTB ^= (1<< PB0);
    uart_printstr("Hello World!\r\n");
}

// ISR UART → Envoie les caractères un par un
ISR(USART_UDRE_vect) {
//	PORTB ^= (1<< PB1);
    if (uart_buffer[uart_index]) {
        UDR0 = uart_buffer[uart_index++];
    } else {
        UCSR0B &= ~(1 << UDRIE0);  // Désactive l'interruption une fois terminé
    }
}

int	main(void){
	uart_init();
	timer_init();
	sei();
//	DDRB |= (1 << PB0) | (1 << PB1); // Debug
//	_delay_ms(DELAY);
	// TODO: Implement Timer1 in mode CTC with interrupt every 2 sec
	while(1){
	}
	return (0);
}