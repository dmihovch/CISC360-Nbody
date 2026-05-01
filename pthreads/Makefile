xorg: main.c src/physics.c src/drawing.c src/bodies.c src/maths.c
	gcc -o thread-impl main.c src/drawing.c src/physics.c src/bodies.c src/maths.c -lraylib -lm -ldl -pthread

xorg-op: main.c src/physics.c src/drawing.c src/bodies.c src/maths.c
	gcc -o thread-impl-o3 main.c src/drawing.c src/physics.c src/bodies.c src/maths.c -lraylib -lm -ldl -pthread -O3

wayland: main.c src/physics.c src/drawing.c src/bodies.c src/maths.c
	gcc -o thread-impl main.c src/drawing.c src/physics.c src/bodies.c src/maths.c -lraylib -lm -ldl -pthread -lX11

wayland-op: main.c src/physics.c src/drawing.c src/bodies.c src/maths.c
	gcc -o thread-impl-o3 main.c src/drawing.c src/physics.c src/bodies.c src/maths.c -lraylib -lm -ldl -pthread -lX11 -O3
	
clean: thread-impl 
	rm thread-impl 

clean-op: thread-impl-o3
	rm thread-impl-o3

run: thread-impl
	./thread-impl

run-op: thread-impl-o3
	./thread-impl-o3

valgrind: thread-impl
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose ./thread-impl


valgrind-op: thread-impl-o3
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose ./thread-impl-o3
