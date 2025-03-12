#ifndef USART_H
# define USART_H

# include <avr/io.h>
# include <avr/interrupt.h>

# define BAUD 115200
# define STRMAX 10
//# define F_CPU 16000000UL

// Initialisation de l'USART
void uart_init(void);

// Envoi d'un caractère
void uart_tx(char c);

// Envoi d'une chaîne de caractères
void uart_printstr(const char *str);

// Lis une chaine de caractere (len definie par STRMAX)
char *uart_readstr(void);
#endif
