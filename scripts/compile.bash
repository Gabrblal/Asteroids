#!/bin/bash

gcc -o bin/Asteroids -Isrc $(find src -name '*.c') $(sdl2-config --cflags --libs)
