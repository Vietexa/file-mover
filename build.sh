#!/bin/bash

cmake -DCMAKE_C_COMPILER=clang -DCMAKE_POLICY_VERSION_MINIMUM=3.5 -S . -B build
cmake --build build