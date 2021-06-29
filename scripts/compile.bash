#!/bin/bash

gcc -g -o bin/Asteroids \
    -Isrc $(find src -name '*.c') \
    $(sdl2-config --cflags --libs) \
    -Wall -Werror -Wpedantic

exit $?
