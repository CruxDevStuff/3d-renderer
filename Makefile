CFLAGS = `sdl2-config --cflags` 
LDFLAGS = `sdl2-config --libs` 

build:
	clang -g $(CFLAGS) $(LDFLAGS) -Wall -std=c99 -Iinclude ./src/*.c -o renderer 

run:
	./renderer

clean:
	rm renderer