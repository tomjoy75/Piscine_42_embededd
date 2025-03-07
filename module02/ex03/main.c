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
	while (!(UCSR0A & (1 << UDRE0)));
	UDR0 = c;
}

ISR(USART_RX_vect){ // Interruption declenched by an input char
	c = UDR0;
	if (c >= 'a' && c <= 'z')
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

// 1-Send back char with interrupts
// 2-Manage /uppercase