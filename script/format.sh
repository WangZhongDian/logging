#!/bin/bash
# Run clang-format on all C/C++ files in this directory and below.
find . -name "*.c" -o -name "*.h" -exec clang-format -style=file -i {} +