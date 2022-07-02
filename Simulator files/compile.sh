#!/bin/sh

clang -g -O0 main.c functions.c
if [[ $? -ne 0 ]]
then
    echo "Compilation failed"
else
    echo "Compilation succeded"
fi
