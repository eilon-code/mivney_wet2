#!/bin/bash

./make_zip.sh wet2out.zip wet2util.h main24b2.cpp code/

cd data_structures_wet2_tests/Tests

chmod +x compile.sh new.sh run.sh
dos2unix compile.sh new.sh run.sh

./new.sh

./compile.sh ./../../wet2out.zip

./run.sh
