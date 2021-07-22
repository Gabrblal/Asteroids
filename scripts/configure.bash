#!/bin/bash

cd ..

# Install dependencies
sudo apt-get install libsdl2-dev
sudo apt-get install libsdl2-image-dev
sudo apt-get install libsdl2-mixer-dev
sudo apt-get install libsdl2-ttf-dev

mkdir lib &>/dev/null
mkdir bin &>/dev/null

wget 'https://www.libsdl.org/release/SDL2-devel-2.0.14-mingw.tar.gz' -O SDL.tar.gz
tar -vxf SDL.tar.gz -C lib
rm SDL.tar.gz

mv lib/SDL2-2.0.14/i686-w64-mingw32 lib/SDL
rm -rf lib/SDL2-2.0.14
