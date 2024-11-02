#!/bin/bash
cmake build -B build . && cd build && make && ctest