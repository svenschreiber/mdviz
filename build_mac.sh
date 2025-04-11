#!/bin/sh

mkdir -p bin
#cp ../hpsc/out/lib/libMDlib.dylib lib
#cp ../hpsc/src/lib/mdlib.h include
clang -O3 -o "bin/mdviz" src/main.c -Iinclude/ -Llib/ -rpath lib -lglfw -framework IOKit -framework Cocoa -framework OpenGL lib/libMDlib.dylib
