CC = g++
CFLAGS = -Wall -I/usr/local/include -L/usr/local/lib  -lsfml-graphics -lsfml-window -lsfml-system

all: 
	$(CC) main.cpp -o gravitas.out $(CFLAGS)
