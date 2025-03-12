#include "usart.h"

void uart_init(void){
	int	baud_prescale = (uint16_t)(( F_CPU /16./BAUD) - 0.5);
	UBRR0H = (baud_prescale >> 8);
	UBRR0L = baud_prescale;
	UCSR0B = (1 << TXEN0) | (1 << RXEN0) | (1 << RXCIE0);  // Activate Tx and Rx and interrupt when a char is received
	UCSR0C = (1 << UCSZ01) | (1 << UCSZ00); // 8 bits mode
    //UCSR0B |= (1 << UDRIE0) | (1 << RXCIE0);  // Active l’interruption d’envoi UART et quand un caractere est recu
	sei();
}

// Envoi d'un caractère
void uart_tx(char c){
	while (!(UCSR0A & (1 << UDRE0)));
	UDR0 = c;
}

// Envoi d'une chaîne de caractères
void uart_printstr(const char *str){
    while (*str) {
        uart_tx(*str++);
    }
}