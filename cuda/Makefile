all: main.c src/physics.c src/drawing.c src/bodies.c src/maths.c
	nvcc -o cuda-impl main.c src/drawing.c src/physics.c src/bodies.c src/maths.c -lraylib -lm -ldl -pthread --cuda-gpu-arch=sm_86

wayland: main.cu src/physics.cpp src/drawing.cpp src/bodies.cpp src/maths.cpp
	nvcc -o cuda-impl main.cu src/drawing.cpp src/physics.cpp src/bodies.cpp src/maths.cpp -lpthread -lraylib -lm -ldl -lX11 -arch=sm_86

clean: cuda-impl 
	rm cuda-impl 

run: cuda-impl
	./cuda-impl
