CFLAGS = `sdl2-config --cflags` 
LDFLAGS = `sdl2-config --libs` 

build:
	clang $(CFLAGS) $(LDFLAGS) -Wall -std=c99 ./src/*.c -o renderer 

run:
	./renderer

clean:
	rm renderer