#!/bin/sh

clang src/*.c -Ilib/ -o "bin/hpsc_project" -Llib/ -framework CoreVideo -framework IOKit -framework Cocoa -framework GLUT -framework OpenGL lib/libraylib.a
