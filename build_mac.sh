#!/bin/sh

#clang -g -O1 -fsanitize=address -o "bin/hpsc_project" src/main.c -Iinclude/ -lglfw -framework IOKit -framework Cocoa -framework OpenGL 
clang -g -O0 -o "bin/hpsc_project" src/main.c -Iinclude/ -Llib/ -lglfw -framework IOKit -framework Cocoa -framework OpenGL
