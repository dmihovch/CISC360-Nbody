all: main.c src/physics.c src/drawing.c src/bodies.c src/maths.c
	gcc -o pthreads-impl main.c src/drawing.c src/physics.c src/bodies.c src/maths.c -lraylib -lm -ldl -pthread

wayland: main.c src/physics.c src/drawing.c src/bodies.c src/maths.c
	gcc -o pthreads-impl main.c src/drawing.c src/physics.c src/bodies.c src/maths.c -lraylib -lm -ldl -pthread -lX11

clean: pthreads-impl 
	rm pthreads-impl 

run: pthreads-impl
	./pthreads-impl

valgrind: pthreads-impl 
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose ./pthreads-impl
