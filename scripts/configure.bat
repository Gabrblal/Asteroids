cd ..

@REM Make binary folder.
(rd /S /Q bin && mkdir bin) || mkdir bin
(rd /S /Q lib && mkdir lib) || mkdir lib

powershell -c "Invoke-WebRequest -Uri 'https://www.libsdl.org/release/SDL2-devel-2.0.14-mingw.tar.gz' -OutFile 'SDL.tar.gz'"

tar -vxf SDL.tar.gz -C lib
del SDL.tar.gz

move lib/SDL2-2.0.14/i686-w64-mingw32 lib/SDL
cd lib &&  rd /S /Q SDL2-2.0.14