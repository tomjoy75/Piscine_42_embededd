#ifndef timer1_H
#define timer1_H

#include <avr/io.h>
#include <avr/interrupt.h>

// ✅ Définition du prescaler (ex: 1024 pour une horloge plus lente)
#define timer1_PRESCALER 1024

// ✅ Fonction pour initialiser le Timer1 en mode CTC
//void timer1_init(uint16_t freq);

// ✅ Fonction pour démarrer le Timer
void timer1_start(void);

// ✅ Fonction pour arrêter le Timer
void timer1_stop(void);

#endif
