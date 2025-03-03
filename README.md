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

