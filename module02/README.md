# 📌 Fiche Résumé – Protocole UART sur AVR (ATmega328P)

## 1️⃣ Introduction à l’UART

L’**UART (Universal Asynchronous Receiver Transmitter)** est un protocole de communication série **asynchrone** permettant d’échanger des données entre microcontrôleurs et périphériques (PC, capteurs, modules Bluetooth, etc.).

✅ **Avantages** :

- Simple à implémenter (2 fils : TX et RX).
- Pas besoin d’horloge externe.
- Supporté nativement par les microcontrôleurs AVR.

⚠ **Limitations** :

- La transmission est **asynchrone**, donc la vitesse doit être bien définie des deux côtés.
- Pas de détection d’erreur avancée (contrairement à SPI ou I2C).

### 🛠 **Broches utilisées sur ATmega328P**

| **Broche**    | **Rôle**                 |
| ------------- | ------------------------ |
| **PD0 (RXD)** | Réception des données    |
| **PD1 (TXD)** | Transmission des données |

---

## 2️⃣ Configuration de l’UART

### 📌 Paramètres essentiels

1. **Baud rate** : vitesse de communication (ex: 9600, 115200 bps).
2. **Format des données** : nombre de bits (7, 8 bits).
3. **Parité** : aucune (`None`), paire (`Even`), impaire (`Odd`).
4. **Bits d’arrêt** : 1 ou 2 (généralement 1).

### 📌 Initialisation de l’UART (Exemple à 9600 bps)

```c
#define F_CPU 16000000UL // Fréquence du microcontrôleur
#define BAUD 9600
#define BAUD_PRESCALE ((F_CPU / (16UL * BAUD)) - 1)

void UART_init(void) {
    UBRR0H = (BAUD_PRESCALE >> 8); // Configurer baud rate
    UBRR0L = BAUD_PRESCALE;
    UCSR0B = (1 << TXEN0) | (1 << RXEN0); // Activer TX et RX
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00); // Mode 8 bits, 1 bit stop
}
```

---

## 3️⃣ Envoi et Réception de Données

### 📌 Envoi d’un caractère via UART

```c
void UART_transmit(char data) {
    while (!(UCSR0A & (1 << UDRE0))); // Attendre que le buffer soit vide
    UDR0 = data; // Envoyer le caractère
}
```

### 📌 Réception d’un caractère via UART

```c
char UART_receive(void) {
    while (!(UCSR0A & (1 << RXC0))); // Attendre que des données arrivent
    return UDR0; // Lire le caractère reçu
}
```

### 📌 Envoi d’une chaîne de caractères

```c
void UART_transmit_string(char* str) {
    while (*str) {
        UART_transmit(*str);
        str++;
    }
}
```

---

## 4️⃣ Utilisation avec un PC (via USB-UART)

Pour connecter un **ATmega328P** à un **PC** via UART, un convertisseur **USB-Série (ex: CP2102, CH340G, FTDI)** est nécessaire.

### 📌 Commandes pour communiquer avec l’ATmega328P sous Linux/Mac :

```sh
screen /dev/ttyUSB0 9600 # Ouvrir une communication série
```

Si `screen` n’est pas installé, utiliser :

```sh
sudo apt install screen  # Debian/Ubuntu
brew install screen      # macOS
```

---

## 🚀 Résumé des Registres UART (ATmega328P)

| **Registre**      | **Rôle**                                          |
| ----------------- | ------------------------------------------------- |
| **UBRR0H/UBRR0L** | Configurer la vitesse de transmission (baud rate) |
| **UCSR0A**        | Statut de transmission (TX/RX prêt)               |
| **UCSR0B**        | Activation TX/RX et interruptions                 |
| **UCSR0C**        | Format des données (8 bits, parité)               |
| **UDR0**          | Buffer de données pour l’envoi/réception          |

---

## 🚀 Exercice Pratique

💡 **Faire clignoter une LED lorsqu'un caractère spécifique est reçu en UART.**

```c
#include <avr/io.h>
#define F_CPU 16000000UL
#define BAUD 9600
#define BAUD_PRESCALE ((F_CPU / (16UL * BAUD)) - 1)

void UART_init(void) {
    UBRR0H = (BAUD_PRESCALE >> 8);
    UBRR0L = BAUD_PRESCALE;
    UCSR0B = (1 << TXEN0) | (1 << RXEN0);
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}

char UART_receive(void) {
    while (!(UCSR0A & (1 << RXC0)));
    return UDR0;
}

int main(void) {
    UART_init();
    DDRB |= (1 << PB0); // Configurer PB0 en sortie
    while (1) {
        if (UART_receive() == 'A') {
            PORTB ^= (1 << PB0); // Inverser l'état de la LED
        }
    }
    return 0;
}
```

---

## 🚀 À retenir

✔ **UART permet une communication simple entre microcontrôleur et PC/périphérique.**\
✔ **Utilise **``** pour envoyer et recevoir des données.**\
✔ **Un convertisseur USB-Série est nécessaire pour interfacer un PC.**\
✔ **Le baud rate doit être identique entre les appareils communicants.**

📌 **L’UART est une brique essentielle pour la communication série en embarqué !** 🚀😃


