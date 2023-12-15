@echo off
gcc -o fsm-test src/fsm-test.c src/dstructs/hashmap/hashmap.c src/dstructs/fsm/fselements.c 
fsm-test.exe 