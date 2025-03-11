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

// Fonctions pour les LEDs classiques (PORTB)
void led_init(void);
void led_on(uint8_t led);
void led_off(uint8_t led);
void led_toggle(uint8_t led);

// Fonctions pour les LEDs RGB (PORTD)
void led_rgb_init(void);
void led_rgb_on(uint8_t led);
void led_rgb_off(uint8_t led);
void led_rgb_toggle(uint8_t led);
void led_rgb_clear(void);

// Effet lumineux simple
void led_blink(void);

// Effet lumineux avancé
void led_show(void);

#endif
