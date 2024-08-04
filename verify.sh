#!/bin/bash

./make_zip.sh wet2out.zip wet2util.h main24b2.cpp code/

python3 "wet-verify.py" wet2out.zip
