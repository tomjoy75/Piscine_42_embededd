#ifndef LED_H
#define LED_H

#include <avr/io.h>
#include <util/delay.h>

# define DELAY_LED 500 // 500ms
// Définition des LEDs sur PORTB
#define D1 PB0
#define D2 PB1
#define D3 PB2
#define D4 PB4

// Définition des LEDs RGB sur PORTD
#define LED_B PD3
#define LED_R PD5
#define LED_G PD6

// Initialisation des LEDs
void led_init(void);

// Allumer une LED spécifique (PORTB ou PORTD)
void led_on(uint8_t port, uint8_t led);

// Éteindre une LED spécifique (PORTB ou PORTD)
void led_off(uint8_t port, uint8_t led);

// Inverser l'état d'une LED (toggle) (PORTB ou PORTD)
void led_toggle(uint8_t port, uint8_t led);

// Effet lumineux simple
void led_blink(void);

// Outil pour la gestion du delay
static void delay_ms_variable(int delay);

// Effet lumineux avancé
void led_show(void);

#endif
