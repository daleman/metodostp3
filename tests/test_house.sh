#!/bin/bash

cd src_cpp

g++ tests/testHouse.cpp ./matriz.h -o ../ejecutables/test_house.exe

cd ..

./ejecutables/test_house.exe
