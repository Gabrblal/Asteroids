cd ../src && ^
dir /s /b | ^
findstr /R .*\.c$ > ../source.txt && cd ..

@REM gcc @file is terrible and ignores backslashes. Replace all '\' with '\\'.
@REM This also means we can't pipe files from findstr into gcc.
powershell -c "(gc source.txt) -replace '\\', '\\' | Out-File -encoding ASCII 'source.txt'"

gcc @source.txt -g -o bin/Asteroids.exe -static -static-libgcc ^
-Isrc -Ilib/SDL/include -Llib/SDL/lib ^
-lmingw32 -lSDL2main -lSDL2 -lm -ldinput8 -ldxguid -ldxerr8 ^
-luser32 -lgdi32 -lwinmm -limm32 -lole32 -loleaut32 ^
-lshell32 -lversion -luuid -lhid -lsetupapi ^
-Wall -Werror -Wpedantic

IF %errorlevel% NEQ 0 (
    del source.txt
    EXIT /B 1
)
ELSE (
    del source.txt
    EXIT /B 0
)
