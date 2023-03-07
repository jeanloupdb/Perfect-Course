CC = gcc
CFLAGS = -Wall -I. -lSDL -lSDL_image
OBJ = virtual.o tool_img.o
DEPS = tool_img.h
BIN = virtual

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

$(BIN): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean

clean:
	rm -f $(BIN) $(OBJ)
