import re
symbols = ["$end", "error", "IDENT", "STR", "CHR", "BOOL", "NIL",
             "INT", "DBL", "INDENT", "DEDENT", "GLOBAL", "FIXED",
             "FLEX", "INPUT", "OUTPUT", "WHEN", "ELSE", "LOOP",
             "AND", "OR", "EQUALS", "NOTEQUALS", "GREATER",
             "GREATEREQUAL", "LESS", "LESSEQUAL", "PLUS",
             "MINUS", "TIMES", "DIVIDE", "MODULO", "POW",
             "NOT", "ABS", "COLON", "EQUAL", "(", ")", "&", "in",
             "to", "by", "int", "dbl", "str", "chr", "bool", "nil",
             "true", "false","accept", "program", "stmts", "stmt", "decl_stmt",
                "mut_type", "assign_stmt", "value", "input_stmt", 
                "output_stmt", "string_consts", "string_const", 
                "when_stmt", "else_stmt", "loop_stmt", "data_type",
                "expression", "conjunction", "equality",
                "relational", "abs", "term", "factor",
                "power", "unary", "literal"]

# 77 x 149 table
states = []


for i in range(153):
    states.append([])
    for j in range(77):
        states[i].append("X")

with open("STATES-2.txt", "r") as f:
    txt = f.read()
    txt_len = len(txt)
    current_pos = 0
    current_state = 0

    while current_pos < txt_len:
        if txt[current_pos].isspace():
            current_pos+=1
            continue

        # if new state
        if txt[current_pos] == '@':
            current_pos += 1

            while txt[current_pos].isdigit() or txt[current_pos].isspace():
                current_pos += 1

            next_state_idx = txt[current_pos:].find("@")

            if next_state_idx > 0:
                matches = re.findall("\S+", txt[current_pos:next_state_idx + current_pos])
            else:
                matches = re.findall("\S+", txt[current_pos:])

            for i in range(0, len(matches), 2):
                 
                if matches[i] == "$default":
                    for idx, cell in enumerate(states[current_state]):
                        if cell == "X" and idx < 51:
                            states[current_state][idx] = matches[i+1]
                else:
                    states[current_state][symbols.index(matches[i])] = matches[i+1]

            current_state += 1
        current_pos += 1


print("{", end="")
for state in states:
    print("{", end="")
    for idx, symbol in enumerate(state):
        if idx == len(state) - 1:
            print(symbol, end="},\n")
        else:
            print(symbol, end=", ")
print("}")
    