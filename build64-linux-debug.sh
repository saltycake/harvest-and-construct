#! /bin/sh

sources="src/main.c.precaked"
search_dirs="-Iinclude/ -Llib/linux64/"
libs="-lSDL2main -lSDL2"

./precake src/main.c src/stringstuff.h
clang-5.0 -x c -std=c11 -ferror-limit=1 -Wall -g -O0 -m64 \
  -o bin/linux64/harcon64-debug $search_dirs $sources $libs
