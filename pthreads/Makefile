all: main.c src/physics.c src/drawing.c src/bodies.c src/maths.c
	gcc -o serial-impl main.c src/drawing.c src/physics.c src/bodies.c src/maths.c -lraylib -lm -ldl -pthread

wayland: main.c src/physics.c src/drawing.c src/bodies.c src/maths.c
	gcc -o serial-impl main.c src/drawing.c src/physics.c src/bodies.c src/maths.c -lraylib -lm -ldl -pthread -lX11



way-op: main.c src/physics.c src/drawing.c src/bodies.c src/maths.c
	gcc -o serial-impl-o3 main.c src/drawing.c src/physics.c src/bodies.c src/maths.c -lraylib -lm -ldl -pthread -lX11 -O3
	
clean: serial-impl 
	rm serial-impl 

run: serial-impl
	./serial-impl

valgrind: serial-impl 
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose ./serial-impl
