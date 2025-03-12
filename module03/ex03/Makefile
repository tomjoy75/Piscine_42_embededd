NAME  = main
CC = avr-gcc
CPU = atmega328p
F_CPU = 16000000UL
BAUD = 115200
PORT = /dev/ttyUSB0
LIB = ./utils/

SRC = $(NAME).c $(LIB)usart.c $(LIB)led.c $(LIB)timer1.c  # ✅ Ajout des fichiers source
#OBJ = $(SRC:.c=.o)  # ✅ Génère la liste des fichiers objets (.o)
OBJ = $(NAME).o $(LIB)usart.o $(LIB)led.o $(LIB)timer1.o


CFLAGS = -mmcu=$(CPU) -DF_CPU=$(F_CPU) -Os  # ✅ Options de compilation
LDFLAGS =  # ✅ Pas de librairies additionnelles pour l'instant

all: flash  # ✅ Compilation et flash en une seule commande

$(NAME).elf: $(OBJ)  
	@echo "🔗✨ Linker tous les objets"
	$(CC) $(CFLAGS) -o $(NAME).elf $(OBJ) $(LDFLAGS)  

utils/%.o: utils/%.c
	@echo "🛠️📦 Compilation séparée de chaque fichier source"
	$(CC) $(CFLAGS) -c $< -o $@  

hex: $(NAME).elf
	@echo "📦➡️🔢 Conversion en format .hex"
	avr-objcopy -O ihex $(NAME).elf $(NAME).hex  

flash: hex
	@echo "🚀⚡ Flash via avrdude" 
	avrdude -c arduino -p m328p -U flash:w:$(NAME).hex:i -P $(PORT) -b $(BAUD)  

clean:
	@echo "🧹🗑️ Suppression propre des fichiers objets et binaires"
	rm -f *.hex *.elf *.o
	rm -f $(LIB)*.o   
