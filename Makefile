TARGET = teturisu
CC=cc
LD=ld
CFLAGS = -std=c99 -g -Wall -Wextra `sdl2-config --cflags`
LFLAGS = `sdl2-config --libs` -lSDL2_image -lSDL2_mixer
SRC = $(wildcard *.c)
OBJ = $(patsubst %.c,%.o,$(SRC))
DEP = $(patsubst %.c,%.d,%(SRC))

$(TARGET): $(OBJ)
	$(CC) $(LFLAGS) $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

%.d: %.c
	$(CC) -MM $< > $@

clean:
	@rm -f *.o *.d $(TARGET)
