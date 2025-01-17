#!/bin/sh

cp ../hpsc/out/lib/libMDlib.dylib lib
cp ../hpsc/src/lib/mdlib.h include
clang -g -O3 -o "bin/hpsc_project" src/main.c -Iinclude/ -Llib/ -rpath lib -lglfw -framework IOKit -framework Cocoa -framework OpenGL lib/libMDlib.dylib
