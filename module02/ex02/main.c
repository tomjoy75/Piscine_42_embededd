#include "main.h"

void	uart_init(void){
	int	baud_prescale = (int)( F_CPU / (16.0 * BAUD) - 0.5);
	UBRR0H = (baud_prescale >> 8);
	UBRR0L = baud_prescale;
	UCSR0B = (1 << TXEN0) | (1 << RXEN0) ;  // Activate Tx and Rx
	UCSR0C = (1 << UCSZ01) | (1 << UCSZ00); // 8 bits mode
}

void	uart_tx(char c){
	UDR0 = c;
}

char	uart_rx( void ){
	while (!(UCSR0A & (1 << RXC0))); // 
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