#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>


void timer1_init(void) {
    DDRB |= (1 << PB1);  // PB1 en sortie (OC1A)

    // Mode 14 : Fast PWM, TOP = ICR1
    TCCR1A |= (1 << COM1A1) | (1 << WGM11);
    TCCR1B |= (1 << WGM13) | (1 << WGM12) | (1 << CS11);  // Prescaler 8

    ICR1 = 19999;  // Fréquence PWM (16 MHz / (8 * 20000) ≈ 100 Hz)
}

void timer0_init(void) {
    TCCR0A |= (1 << WGM01);  // Mode CTC
    TCCR0B |= (1 << CS02) | (1 << CS00);  // Prescaler 1024

    OCR0A = 2;  // Interruption toutes les 1 ms (16 MHz / 1024 / 16)
    TIMSK0 |= (1 << OCIE0A);  // Active l'interruption sur OCR0A
}

volatile int duty_cycle = 0;
volatile int direction = 1;  // 1 = augmente, -1 = diminue

ISR(TIMER0_COMPA_vect) {
    duty_cycle += direction;  // Augmente ou diminue le rapport cyclique

    if (duty_cycle >= ICR1) {
        duty_cycle = ICR1;
        direction = -5;  // Inverser la direction (descente)
    } else if (duty_cycle <= 0) {
        duty_cycle = 0;
        direction = 5;  // Inverser la direction (montée)
    }

    OCR1A = duty_cycle;  // Mise à jour du rapport cyclique
}

int main(void) {
    timer1_init();
    timer0_init();
    sei();  // Active les interruptions

    while (1) {
        // Tout est géré par les interruptions
    }
}

