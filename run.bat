@echo off
gcc -o czar czar.c lexer/lexer.c utils/utils.c utils/stack.c
czar.exe %1 %2