# Piscine_42_embededd
42 Project to discover embedded programming

## Port Registers
DDR(B/D/C), PORT(B/D/C), PIN(B/D/C) are called port registers

DDRB is the Data Direction register for port “B”. This means that if you set this register to 0xFF (by running DDRB |= 0xFF ), all ports or pins in the “B” I/O port act as outputs. If you set DDRB to 0x00 (it’s initialized to 0x00 by default), then ports or pins in the “B” I/O port act as inputs.

Each bit control the direction 
```
Bit :  7  6  5  4  3  2  1  0  
Broche: PB7 PB6 PB5 PB4 PB3 PB2 PB1 PB0
```  
0 = input \
1 = output

https://www.quora.com/What-is-DDRB-PORTB-and-PINB-and-what-do-they-do-What-does-it-mean-if-a-port-or-pin-is-an-output-pin


## 🚀 Résumé rapide des opérations binaires  

| **Action** | **Opération** | **Exemple (`n=2`)** |
|------------|-------------|----------------------|
| **Mettre le bit `n` à `1`** | `X |= (1 << n);` | `X |= 0b00000100;` |
| **Mettre le bit `n` à `0`** | `X &= ~(1 << n);` | `X &= 0b11111011;` |
| **Inverser le bit `n`** | `X ^= (1 << n);` | `X ^= 0b00000100;` |
| **Tester si le bit `n` est `1`** | `if (X & (1 << n))` | `if (X & 0b00000100)` |
| **Tester si le bit `n` est `0`** | `if (!(X & (1 << n)))` | `if (!(X & 0b00000100))` |
| **Effacer plusieurs bits** | `X &= ~MASK;` | `X &= 0b11110011;` |
| **Mettre plusieurs bits à `1`** | `X |= MASK;` | `X |= 0b00001100;` |
| **Inverser plusieurs bits** | `X ^= MASK;` | `X ^= 0b00001100;` |
| **Décaler à gauche (`×2`)** | `X <<= n;` | `X <<= 2;` |
| **Décaler à droite (`÷2`)** | `X >>= n;` | `X >>= 2;` |

## For the clock

https://qiriro.com/bme6163/static_files/notes/L3/Newbie's%20Guide%20to%20AVR%20Timers.pdf
