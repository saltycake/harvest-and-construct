#! /bin/sh

sources="src/main.c.precaked"
search_dirs="-Iinclude/ -Llib/win64/"
libs="-lmingw32 -lpthread -lm -lSDL2main -lSDL2 -lopengl32"
platform="--target=x86_64-w64-mingw32 -mwindows -m64"

./precake src/main.c src/stringstuff.h
clang-5.0 -x c -std=c11 -ferror-limit=1 -Wall -g -O0 $platform \
  -o bin/win64/harcon64-debug.exe $search_dirs $sources $libs
