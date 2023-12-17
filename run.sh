#!/bin/bash
gcc -o czar czar.c lexer/lexer.c utils/utils.c utils/stack.c
./czar $1 $2