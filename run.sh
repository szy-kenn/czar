#!/bin/bash
gcc -o czar src/czar.c src/dstructs/hashmap/hashmap.c src/dstructs/fsm/fsmachine.c src/dstructs/fsm/fselements.c src/dstructs/fsm/fsutils.c src/memory_manager/memory_manager.c src/charset/charset.c
./czar $1 $2