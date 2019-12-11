all:	
	gcc `sdl2-config --cflags --libs` *.c -o teturisu