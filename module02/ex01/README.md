# 📌 Fiche Résumé – Registres et Pins utilisés (ATmega328P)

## 1️⃣ Introduction
Ce document récapitule les **registres** et **pins** utilisés dans le programme pour la gestion de :
- **L'UART (communication série)**
- **Le Timer1 (génération d'interruptions toutes les 2 secondes)**

---

## 2️⃣ Tableau récapitulatif des registres et pins

| **Registre** | **Bits utilisés** | **Pins concernées** | **Rôle** |
|-------------|------------------|--------------------|----------|
| **UBRR0H / UBRR0L** | Baud rate (UBRR) | - | Configure la vitesse de communication série (115200 bauds). |
| **UCSR0A** (Control and Status Register A) | `UDRE0` (bit 5) | - | Indique si le registre de transmission UART (`UDR0`) est prêt à envoyer un nouveau caractère. |
| **UCSR0B** (Control and Status Register B) | `TXEN0` (bit 3), `UDRIE0` (bit 5) | - | Active l’émetteur UART (`TXEN0`) et l'interruption de transmission (`UDRIE0`). |
| **UCSR0C** (Control and Status Register C) | `UCSZ01` (bit 2), `UCSZ00` (bit 1) | - | Configure le format des données (8 bits, 1 stop, pas de parité). |
| **UDR0** (UART Data Register) | 8 bits de données | **PD1 (TX)**, **PD0 (RX)** | Contient le caractère à envoyer ou reçu via l’UART. |
| **TCCR1B** (Timer1 Control Register B) | `WGM12` (bit 3), `CS12` (bit 2), `CS10` (bit 0) | - | Configure Timer1 en mode CTC (WGM12) avec un prescaler de 1024 (`CS12` + `CS10`). |
| **OCR1A** (Output Compare Register A) | Valeur de 31247 | - | Définit la valeur à atteindre pour générer une interruption toutes les 2 secondes. |
| **TIMSK1** (Timer Interrupt Mask) | `OCIE1A` (bit 1) | - | Active l’interruption quand Timer1 atteint `OCR1A`. |
| **DDRB** (Data Direction Register B) | `PB0`, `PB1`, `PB2` | **PB0, PB1, PB2** | Définit ces broches comme des sorties. |

---

## 3️⃣ Explication des principaux registres et interactions

### 🔹 **1. UART (Communication série)**
- **UBRR0H / UBRR0L** : Définit la vitesse de transmission à **115200 bauds**.
- **UCSR0B** :
  - `TXEN0` → Active l’émetteur UART.
  - `UDRIE0` → Active l’interruption quand `UDR0` est vide.
- **UCSR0C** : Définit **8 bits de données**, **1 bit stop**, **pas de parité**.
- **UDR0** : Contient le caractère en cours de transmission.

### 🔹 **2. Timer1 (Interruption toutes les 2 secondes)**
- **TCCR1B** :
  - `WGM12` → Active le mode CTC (Clear Timer on Compare Match).
  - `CS12` + `CS10` → Configure un **prescaler de 1024**.
- **OCR1A** : Valeur définie pour **atteindre 2 secondes**.
- **TIMSK1** : Active l’**interruption Timer1** (`TIMER1_COMPA_vect`).

---

## 4️⃣ Résumé rapide
| **Module** | **Registres clés** | **Utilité** |
|-----------|----------------|-----------|
| **UART (Série)** | `UBRR0H/L`, `UCSR0A/B/C`, `UDR0` | Communication avec le PC (115200 bauds). |
| **Timer1** | `TCCR1B`, `OCR1A`, `TIMSK1` | Déclenche `uart_printstr("Hello World!\r\n")` toutes les 2 secondes. |

---

📌 **Ce document permet de mieux comprendre les registres impliqués dans le fonctionnement du programme sur ATmega328P.**
