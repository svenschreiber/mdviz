#!/bin/sh

clang -o "bin/hpsc_project" src/main.c -Iinclude/ -lglfw -framework IOKit -framework Cocoa -framework OpenGL 
