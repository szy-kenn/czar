#!/bin/bash

gcc -o czar src/czar.c src/lexer/lexer.c src/parser/parser.c src/file_handler/file_handler.c src/czar-state-machine/czar-state-machine.c src/czar-state-machine/indentation-state-machine.c src/dstructs/hashmap/hashmap.c src/dstructs/stack/stack.c src/dstructs/fsm/fsmachine.c src/dstructs/fsm/fselements.c src/dstructs/fsm/fsutils.c src/memory_manager/memory_manager.c src/charset/charset.c

./czar $1 $2
