#!/bin/bash

cd src_cpp

g++ tests/test_matriz.cpp ./matriz.h -o ../ejecutables/test_matriz.exe

cd ..

valgrind ./ejecutables/test_matriz.exe
