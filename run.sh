#!/bin/bash
gcc -o czar czar.c lexer/lexer.c utils/utils.c
./czar $1 $2