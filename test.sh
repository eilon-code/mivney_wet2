#!/bin/bash

cd Tests

./../make_zip.sh wet2out.zip wet2util.h main24b2.cpp ../code/

./compile.sh wet2out.zip

./run.sh
