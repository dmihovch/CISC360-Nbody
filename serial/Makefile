all: main.c src/physics.c src/drawing.c src/bodies.c src/maths.c
	gcc -o serial main.c src/drawing.c src/physics.c src/bodies.c src/maths.c -lraylib -lm -ldl -pthread

wayland: main.c src/physics.c src/drawing.c src/bodies.c src/maths.c
	gcc -o serial main.c src/drawing.c src/physics.c src/bodies.c src/maths.c -lraylib -lm -ldl -pthread -lX11

clean: serial 
	rm serial 

run: serial
	./serial

valgrind: serial 
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose ./serial
