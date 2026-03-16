# CISC360-Nbody

Magnus Culley & Dan Mihovch CISC360 Computer Architecture Final Project


# Dependecies:

1. libc
2. CUDA
3. pthreads
4. raylib

# Raylib Install

git clone --depth 1 https://github.com/raysan5/raylib.git raylib
cd raylib/src/
make PLATFORM=PLATFORM_DESKTOP
sudo make install

# Compiling with Raylib

compile your C program as normal, add these flags
-lraylib -lm -ldl -pthread
