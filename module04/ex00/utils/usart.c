#include "usart.h"

volatile char buffer[STRMAX + 1];
volatile uint8_t usart_index = 0;
volatile uint8_t flag_ready = 0;

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

static void	enter_pressed(){
	buffer[usart_index] = '\0';
	flag_ready = 1;
	usart_index = 0;
	uart_printstr("\r\033[1B");
}

static void	backspace_pressed(){
	if (usart_index == 0)
		return; // If beginnig of word, do nothing
	usart_index--;
	buffer[usart_index] = '\0';
	uart_printstr("\033[1D\033[K");
}

ISR(USART_RX_vect){
{	
	char c = UDR0;

	if (c == '\r'){
		enter_pressed();
	}
	else if (c == 127)
		backspace_pressed();
	else if (usart_index >= STRMAX)
		return;
	else{
		buffer[usart_index] = c;
		usart_index++;
		uart_tx(c);
	}
}

}
char *uart_readstr(void){
	while (!flag_ready);
	flag_ready = 0;
	return ((char *)buffer);
}
