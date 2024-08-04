#!/bin/bash

make clean

make -f makefile

valgrind ./build/prog < test/test1.in > test/out.out
