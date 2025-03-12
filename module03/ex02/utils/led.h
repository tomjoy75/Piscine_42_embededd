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

// Couleurs de la LED RGB (combinaison de LED_R, LED_G, LED_B)
#define COLOR_OFF     0
#define COLOR_RED     (1 << LED_R)          // 🔴 Rouge
#define COLOR_GREEN   (1 << LED_G)          // 🟢 Vert
#define COLOR_BLUE    (1 << LED_B)          // 🔵 Bleu
#define COLOR_YELLOW  (1 << LED_R) | (1 << LED_G)  // 🟡 Jaune (Rouge + Vert)
#define COLOR_CYAN    (1 << LED_G) | (1 << LED_B)  // 🔵 Cyan (Vert + Bleu)
#define COLOR_MAGENTA (1 << LED_R) | (1 << LED_B)  // 🟣 Magenta (Rouge + Bleu)
#define COLOR_WHITE   (1 << LED_R) | (1 << LED_G) | (1 << LED_B)  // ⚪ Blanc (Rouge + Vert + Bleu)

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
void led_rgb_set(uint8_t leds);

// 🔔Fonctions pour l'ex02
void init_rgb();
void set_rgb(uint8_t r, uint8_t g, uint8_t b);

// Effet lumineux simple
void led_blink(void);

// Effet lumineux avancé
void led_show(void);

#endif
