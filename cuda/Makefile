all: main.c src/physics.c src/drawing.c src/bodies.c src/maths.c
	gcc -o cuda-impl main.c src/drawing.c src/physics.c src/bodies.c src/maths.c -lraylib -lm -ldl -pthread

wayland: main.c src/physics.c src/drawing.c src/bodies.c src/maths.c
	gcc -o cuda-impl main.c src/drawing.c src/physics.c src/bodies.c src/maths.c -lraylib -lm -ldl -pthread -lX11

clean: cuda-impl 
	rm cuda-impl 

run: cuda-impl
	./cuda-impl

valgrind: cuda-impl 
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose ./cuda-impl
