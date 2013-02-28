.phony: all clean

all: lib tests

lib:
	mkdir -p "./build"
	gcc $(wildcard *.c) -g -shared -std=c99 -O2 -o ./build/libeditor.dll

tests: lib
	cp ./build/libeditor.dll ./tests/libeditor.dll
	gcc tests/test1.c -g -L./bin -leditor -std=c99 -o tests/test1.exe