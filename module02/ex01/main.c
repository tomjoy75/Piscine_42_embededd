#include "main.h"

char	*uart_buffer;
int		uart_index = 0;

void	uart_init(void){
	int	baud_prescale = (int)( F_CPU / (16.0 * BAUD) - 0.5);
	UBRR0H = (baud_prescale >> 8);
	UBRR0L = baud_prescale;
//	UCSR0B = (1 << TXEN0) | (1 << RXEN0); // Activate Tx an Rx
	UCSR0B = (1 << TXEN0) | (1 << UDRIE0) ; // Activate Tx and Interruption of Data reg empty 
	UCSR0C = (1 << UCSZ01) | (1 << UCSZ00); // 8 bits mode
}

void	uart_tx(char c){
	/* Wait for empty transmit buffer */
	while (!(UCSR0A & (1<<UDRE0)))
	;
	UDR0 = c;
}

void uart_printstr(const char* str){
	strncpy(uart_buffer, str, 12);
	uart_index = 0;
	// while (*str){
	// 	uart_tx(*str);
	// 	str++;
	// }
	// uart_tx('\n');
	// UCSR0B = (1 << UDRIE0);
}

int	main(void){
	uart_init();
	uart_tx(*(uart_buffer + uart_index++));
	if (*(uart_buffer + uart_index) == '\0' || *(uart_buffer + uart_index) == '\n'){
		UCSR0B &= ~(1 << UDRIE0); 
		uart_index = 0;
	}
	while(1){
		// uart_printstr("Hello world!");
		// _delay_ms(DELAY);
	}
	return (0);
}