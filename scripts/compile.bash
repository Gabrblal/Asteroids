#!/bin/bash

cd ..

gcc -g -o bin/Asteroids \
    -Isrc $(find src -name '*.c') \
    $(sdl2-config --cflags --libs) -static-libgcc -lm\
    -Wall -Werror -Wpedantic

exit $?
