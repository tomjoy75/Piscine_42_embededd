#include "timer1.h"

// Frequency by second
// void timer1_init(uint16_t freq) {
//     TCCR1B |= (1 << WGM12); // CTC Mode
//     OCR1A = (F_CPU / (timer1_PRESCALER * freq)) - 1;
//     TIMSK1 |= (1 << OCIE1A); // Activate interruption compare OCR1A
//     sei();
// }

void timer1_start(void) {
    // ✅ Appliquer le prescaler et démarrer le Timer1
    TCCR1B |= (1 << CS12) | (1 << CS10);
}

void timer1_stop(void) {
    // ✅ Stopper le Timer (en mettant le prescaler à 0)
    TCCR1B &= ~((1 << CS12) | (1 << CS10));
}
