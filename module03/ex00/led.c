#include "led.h"

void led_init(void) {
    // Configurer les broches PB0, PB1, PB2, PB4 en sortie
    DDRB |= (1 << D1) | (1 << D2) | (1 << D3) | (1 << D4);
}

void led_rgb_init(void){
    // Configurer les broches PD3, PD5, PD6 en sortie (LED RGB)
    DDRD |= (1 << LED_B) | (1 << LED_R) | (1 << LED_G);
}

// Fonctions pour les LEDs sur PORTB (classiques)
void led_on(uint8_t led) {
    PORTB |= 1 << led;  // ✅ Allumer plusieurs LEDs simultanément
}

void led_off(uint8_t led) {
    PORTB &= ~(1 << led);  // ✅ Éteindre plusieurs LEDs simultanément
}

void led_toggle(uint8_t led) {
    PORTB ^= 1 << led;  // ✅ Basculer plusieurs LEDs simultanément
}

//  Fonctions pour les LEDs RGB sur PORTD
void led_rgb_on(uint8_t led) {
    PORTD |= 1 << led;
}

void led_rgb_off(uint8_t led) {
    PORTD &= ~(1 << led);
}

void led_rgb_toggle(uint8_t led) {
    PORTD ^= 1 << led;
}

void led_rgb_clear(void) {
    PORTD &= ~((1 << LED_B) | (1 << LED_R) | (1 << LED_G));  // ✅ Éteindre toutes les LEDs RGB
}


void led_blink(void){
    PORTB |= (1 << D1) | (1 << D2) | (1 << D3) | (1 << D4);
    _delay_ms(DELAY_LED);
    PORTB &= ~((1 << D1) | (1 << D2) | (1 << D3) | (1 << D4)); 
}

static void _delay_ms_variable(int delay) {
    for (int i = 0; i < delay; i++) {
        _delay_ms(1); 
    }
}

void led_show(void){
    int delay = DELAY_LED;
    int nb = 5;
    for (int i = 0; i < nb; i++) {
        led_on(D1);
        _delay_ms_variable(delay);
        led_off(D1);
        
        led_on(D2);
        _delay_ms_variable(delay);
        led_off(D2);
        
        led_on(D3);
        _delay_ms_variable(delay);
        led_off(D3);
        
        led_on(D4);
        _delay_ms_variable(delay);
        led_off(D4);

        if (delay > 10)
            delay -= (int)(delay / 3);
    }
}

