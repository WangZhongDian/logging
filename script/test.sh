#!/bin/bash
cmake build -B build . && cd build && cmake --build . && ctest