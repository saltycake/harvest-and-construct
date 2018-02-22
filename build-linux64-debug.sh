#! /bin/sh

sources="src/main.c"
search_dirs="-Iinclude/ -Llib/linux64/"
libs="-DLINUX64=1 -lSDL2main -lSDL2"

#./precake src/main.c src/stringstuff.h
mkdir -p bin/linux64
clang-5.0 -x c -std=c11 -ferror-limit=1 -Wall -g -O0 -m64 \
  -o bin/linux64/harcon64-debug $search_dirs $sources $libs
