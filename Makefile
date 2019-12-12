all:	
	gcc -g -Wall `sdl2-config --cflags --libs` -lSDL2_image *.c -o teturisu