#!/bin/bash

make clean

make -f makefile

valgrind ./build/prog < test/randtest0.in > test/randtest0.out
