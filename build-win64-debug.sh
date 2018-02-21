#! /bin/sh

sources="src/main.c.precaked"
search_dirs="-Iinclude/ -Llib/win64/"
libs="-lmingw32 -lSDL2main -lSDL2"
platform="-mwindows -m64"

./precake src/main.c src/stringstuff.h
x86_64-w64-mingw32-gcc $platform -x c -std=c11 -fmax-errors=1 -Wall -g -O0 \
  -o bin/win64/harcon64-debug.exe $search_dirs $sources $libs
