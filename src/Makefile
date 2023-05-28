# Variables
CC = gcc
CFLAGS = -Wall -Wextra
LIBS =  -lGL -lGLU -lglut -lm

# Cibles
all: executable

executable: graph.o
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

graph.o: graph.c
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f *.o executable
