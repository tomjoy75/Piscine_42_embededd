# 📌 Gestion des Timers en AVR (ATmega328P)

## 1️⃣ Introduction aux Timers
Un **Timer** est un compteur matériel qui **s'incrémente automatiquement** à chaque cycle d'horloge ou selon un prescaler. Il permet :
- ✅ **Mesurer le temps** (délais précis)
- ✅ **Générer des interruptions périodiques**
- ✅ **Créer des signaux PWM**
- ✅ **Déclencher des actions automatiques sans bloquer le programme**

### 🛠 **Timers disponibles sur ATmega328P**  
| **Timer**  | **Taille** | **Usage principal**       |
|-----------|-----------|----------------------|
| **Timer0** | 8 bits    | Délais courts, PWM  |
| **Timer1** | 16 bits   | Délais longs, PWM, comptage précis |
| **Timer2** | 8 bits    | PWM, génération d'horloge |

---

## 2️⃣ Modes des Timers

### 📌 Mode Normal
- **Le timer compte jusqu'à sa valeur max (`255` pour 8-bit, `65535` pour 16-bit) puis déborde (`overflow`).**
- **Utilisé avec des interruptions pour exécuter une action périodique.**

```c
TCCR1B |= (1 << CS12); // Prescaler = 256, mode normal
while (!(TIFR1 & (1 << TOV1))); // Attendre overflow
TIFR1 |= (1 << TOV1); // Réinitialiser le flag
```

---

### 📌 Mode CTC (Clear Timer on Compare Match)
- **Le timer compte de `0` à `OCRxA`, puis revient à `0`.**
- **Idéal pour générer des fréquences précises.**

```c
TCCR1B |= (1 << WGM12); // Mode CTC
OCR1A = 7811; // (F_CPU / Prescaler * Temps) - 1
TCCR1A |= (1 << COM1A0); // Toggle OC1A (PB1)
TCCR1B |= (1 << CS12) | (1 << CS10); // Prescaler = 1024
```

---

### 📌 Mode PWM (Fast PWM & Phase Correct)
- **Génère des signaux PWM pour le contrôle de moteur, LED dimming, etc.**

```c
TCCR1A |= (1 << COM1A1) | (1 << WGM11); // Mode Fast PWM
TCCR1B |= (1 << WGM12) | (1 << WGM13) | (1 << CS11); // Prescaler = 8
ICR1 = 39999; // Fixer la période
OCR1A = 19999; // Rapport cyclique 50%
```

---

## 3️⃣ Configuration des Timers

### 📌 Sélection du Prescaler

| **Prescaler** | **Bits à configurer (`CSx2:CSx0`)** | **Fréquence Timer1 (F_CPU = 16MHz)** |
|-------------|-------------------|----------------------|
| 1          | `001`              | 16 000 000 Hz |
| 8          | `010`              | 2 000 000 Hz |
| 64         | `011`              | 250 000 Hz |
| 256        | `100`              | 62 500 Hz |
| 1024       | `101`              | 15 625 Hz |

```c
TCCR1B |= (1 << CS12) | (1 << CS10); // Prescaler = 1024
```

---

## 4️⃣ Interruptions des Timers
- **Permettent d'exécuter du code à intervalles précis sans bloquer le CPU.**

```c
ISR(TIMER1_COMPA_vect) {
    PORTB ^= (1 << PB1); // Inverser la LED
}

void setupTimer1() {
    TCCR1B |= (1 << WGM12) | (1 << CS12) | (1 << CS10); // Mode CTC, prescaler 1024
    OCR1A = 15624; // (16MHz / 1024) * 0.1s - 1
    TIMSK1 |= (1 << OCIE1A); // Activer interruption
    sei(); // Activer interruptions globales
}
```

---

## 🚀 Résumé des commandes essentielles

| **Opération**                     | **Commande**                         |
|----------------------------------|---------------------------------|
| Activer un Timer                 | `TCCR1B |= (1 << CS12);`       |
| Choisir Mode CTC                  | `TCCR1B |= (1 << WGM12);`      |
| Choisir Mode PWM                  | `TCCR1A |= (1 << WGM10) | (1 << COM1A1);` |
| Fixer valeur OCR1A                | `OCR1A = 7811;`                |
| Activer interruption CTC          | `TIMSK1 |= (1 << OCIE1A);`     |
| Activer interruptions globales    | `sei();`                       |
| Vérifier overflow                 | `while (!(TIFR1 & (1 << TOV1)));` |

---

### 🚀 **À retenir**
✔ **Utilise le Mode CTC (`WGM12`) pour des signaux précis.**  
✔ **Utilise le Mode PWM (`COM1A1`) pour générer des signaux modulés.**  
✔ **Utilise les interruptions pour éviter les boucles bloquantes (`ISR(TIMER1_COMPA_vect)`).**  

📌 **Les Timers sont ultra puissants, ils permettent d'exécuter du code sans ralentir ton programme !** 🚀😃


