# getting this working was a song and dance. i'll probably try to go back to cmake at some point, but for now, it's makefiles. 
# SDL3 installation followed these isntructions: https://wiki.libsdl.org/SDL3/README-windows
# environment is MSYS2, compiler is mingw-w64 (i think), installation path for SDL3 is C:/Libraries, version is the source code isntalled right from SDL's github releases page (specifically 3.4.

all:
	x86_64-w64-mingw32-g++.exe src/main.cpp src/engine.cpp include/engine.h -Wall -Wpedantic -std=c++20 -g -O0 -I./include -I./external/include -L./external/lib -lSDL3
