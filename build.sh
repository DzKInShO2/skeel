#!/bin/sh
flags="-Wall -Wextra"
gcc -o skeel $(find -type f -name "*.c") $flags
