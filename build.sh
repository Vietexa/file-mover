#!/bin/bash

cmake -DCMAKE_C_COMPILER=clang -S . -B build
cmake --build build