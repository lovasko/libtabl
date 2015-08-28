#!/bin/sh

INC_DIR=/usr/include
LIB_DIR=/usr/lib

cp -v bin/libtabl.so "${LIB_DIR}"
cp -v src/tabl.h "${INC_DIR}"

