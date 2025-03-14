## 📌 Fiche Succincte – Configuration d'un Timer (ATmega328P)

### ✅ **Étapes de Configuration**

1. **Déterminer le mode de fonctionnement** (“Normal”, “CTC”, “PWM”, etc.).  
2. **Choisir le Timer** à utiliser (**Timer0, Timer1 ou Timer2**) en fonction de la précision nécessaire (**8 bits** ou **16 bits**).  
3. **Configurer le mode** dans `TCCRnA` et `TCCRnB` en ajustant les bits `WGMx`.  
4. **Déterminer la fréquence du Timer** avec la formule :  
   ```
   f_{timer} = \frac{F_{CPU}}{Prescaler}
   ```  
5. **Choisir un prescaler** (à définir dans `CSx` de `TCCRnB`).  
6. **Si en mode CTC** → **Définir `OCRnA`** avec :  
   ```
   OCRnA = \frac{F_{CPU}}{Prescaler \times f_{interruption}} - 1
   ```  
7. **Activer les interruptions (éventuel)** en mettant `OCIE0A`, `TOIE1`, etc. dans `TIMSKn`.  
8. **Lancer le Timer** en activant les bits `CSx` dans `TCCRnB`.  
9. **Définir une ISR (éventuel)** → `ISR(TIMERn_COMPA_vect)`.  

---

### ⚡ **Exemple Rapide : Timer0 en CTC avec interruption toutes les 1ms**
```c
TCCR0A |= (1 << WGM01);  // Mode CTC
TCCR0B |= (1 << CS01) | (1 << CS00);  // Prescaler 64
OCR0A = 249;  // (16MHz / 64 / 1000Hz) - 1
TIMSK0 |= (1 << OCIE0A);  // Activer l’interruption
sei();  // Activer les interruptions globales
```

---

✅ **Cette fiche permet une configuration rapide des Timers AVR !** 🚀


