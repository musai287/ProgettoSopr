# Compilatore e flag
CC = gcc
CFLAGS = -Wall -Wextra -g
LDFLAGS = -lncurses

# File sorgenti e oggetti
SRC = main.c croco.c frog.c funzionamento.c piddini.c struct.c collisioni.c proiettili.c
OBJ = $(SRC:.c=.o)

# Nome del file eseguibile
TARGET = frog

# Regola principale: crea l'eseguibile
$(TARGET): $(OBJ)
	$(CC) -o $(TARGET) $(OBJ) $(LDFLAGS)

# Regola per compilare i file .c in .o
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Pulizia dei file compilati
clean:
	rm -f $(OBJ) $(TARGET)

# Pulizia completa (inclusi file di backup)
distclean: clean
	rm -f *~

# Regola "phony" per evitare conflitti con file chiamati come le regole
.PHONY: clean distclean

