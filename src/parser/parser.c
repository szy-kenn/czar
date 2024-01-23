#include "parser.h"
#include <stdio.h>

Parser parser;

Production rules[40] = {
    
    // 0
    {P_ACCEPT, 1, {P_EXPR}},

    // 1 - 2
    {P_EXPR, 1, {P_CONJ}},
    {P_EXP, 3, {P_EXPR, P_OR, P_CONJ}},

    // 3 - 4
    {P_CONJ, 3, {P_CONJ, P_AND, P_EQL}},
    {P_CONJ, 1, {P_EQL}},

    // 5 - 7
    {P_EQL, 3, {P_EQL, P_EQL_EQL, P_REL}},
    {P_EQL, 3, {P_EQL, P_NOT_EQL, P_REL}},
    {P_EQL, 1, {P_REL}},

    // 8 - 12
    {P_REL, 3, {P_REL, P_GREATER, P_TERM}},
    {P_REL, 3, {P_REL, P_GREATER_EQL, P_TERM}},
    {P_REL, 3, {P_REL, P_LESS, P_TERM}},
    {P_REL, 3, {P_REL, P_LESS_EQL, P_TERM}},
    {P_REL, 1, {P_TERM}},

    // 13 - 15
    {P_TERM, 3, {P_TERM, P_PLUS, P_FACTOR}},
    {P_TERM, 3, {P_TERM, P_MINUS, P_FACTOR}},
    {P_TERM, 1, {P_FACTOR}},

    // 16 - 19 
    {P_FACTOR, 3, {P_FACTOR, P_MUL, P_POWER}},
    {P_FACTOR, 3, {P_FACTOR, P_DIV, P_POWER}},
    {P_FACTOR, 3, {P_FACTOR, P_MOD, P_POWER}},
    {P_FACTOR, 1, {P_POWER}},

    // 20-21
    {P_POWER, 3, {P_POWER, P_EXP, P_UNARY}},
    {P_POWER, 1, {P_UNARY}},

    // 22-24 
    {P_UNARY, 2, {P_NOT, P_UNARY}},
    {P_UNARY, 2, {P_MINUS, P_UNARY}},
    {P_UNARY, 1, {P_ABSVAL}},

    // 25-26
    {P_ABSVAL, 3, {P_ABS, P_ABSVAL, P_ABS}},
    {P_ABSVAL, 1, {P_LITERAL}},

    // 27 - 32
    {P_LITERAL, 3, {P_LPAREN, P_EXPR, P_RPAREN}},
    {P_LITERAL, 1, {P_INT}},
    {P_LITERAL, 1, {P_DBL}},
    {P_LITERAL, 1, {P_TRUE}},
    {P_LITERAL, 1, {P_FALSE}},
    {P_LITERAL, 1, {P_ID}},
};

parsing_state parsing_table[100][100] = {
//   $      %	    (	    )	    *	    +	    -	    /	    ^	    |	    error	INT	    DBL	    TRUE	FALSE	ID	    OR	    AND	    NOT 	==	    !=	    >	    >=	    <	    <=	    acc	    expr	conj	eql	    rel	    trm	    fac	    pow	    una	    abs	    lit
    {X,     X,      S(9),   X,      X,      X,      S(7),   X,      X,      S(8),   X,      S(1),   S(2),   S(3),   S(4),   S(5),   X,      X,      S(6),   X,      X,      X,      X,      X,      X,      X,      GT(10), GT(11), GT(12), GT(13), GT(14), GT(15), GT(16), GT(17), GT(18), GT(19)  },
    {R(28), R(28),  R(28),  R(28),  R(28),  R(28),  R(28),  R(28),  R(28),  R(28),  R(28),  R(28),  R(28),  R(28),  R(28),  R(28),  R(28),  R(28),  R(28),  R(28),  R(28),  R(28),  R(28),  R(28),  R(28),  X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X       },
    {R(29), R(29),  R(29),  R(29),  R(29),  R(29),  R(29),  R(29),  R(29),  R(29),  R(29),  R(29),  R(29),  R(29),  R(29),  R(29),  R(29),  R(29),  R(29),  R(29),  R(29),  R(29),  R(29),  R(29),  R(29),  X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X       },
    {R(30), R(30),  R(30),  R(30),  R(30),  R(30),  R(30),  R(30),  R(30),  R(30),  R(30),  R(30),  R(30),  R(30),  R(30),  R(30),  R(30),  R(30),  R(30),  R(30),  R(30),  R(30),  R(30),  R(30),  R(30),  X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X       },
    {R(31), R(31),  R(31),  R(31),  R(31),  R(31),  R(31),  R(31),  R(31),  R(31),  R(31),  R(31),  R(31),  R(31),  R(31),  R(31),  R(31),  R(31),  R(31),  R(31),  R(31),  R(31),  R(31),  R(31),  R(31),  X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X       },
    {R(32), R(32),  R(32),  R(32),  R(32),  R(32),  R(32),  R(32),  R(32),  R(32),  R(32),  R(32),  R(32),  R(32),  R(32),  R(32),  R(32),  R(32),  R(32),  R(32),  R(32),  R(32),  R(32),  R(32),  R(32),  X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X       },
    {X,     X,      S(9),   X,      X,      X,      S(7),   X,      X,      S(8),   X,      S(1),   S(2),   S(3),   S(4),   S(5),   X,      X,      S(6),   X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      GT(20), GT(18), GT(19)  },
    {X,     X,      S(9),   X,      X,      X,      S(7),   X,      X,      S(8),   X,      S(1),   S(2),   S(3),   S(4),   S(5),   X,      X,      S(6),   X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      GT(21), GT(18), GT(19)  },
    {X,     X,      S(9),   X,      X,      X,      X,      X,      X,      S(8),   X,      S(1),   S(2),   S(3),   S(4),   S(5),   X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      GT(22), GT(19)  },
    {X,     X,      S(9),   X,      X,      X,      S(7),   X,      X,      S(8),   X,      S(1),   S(2),   S(3),   S(4),   S(5),   X,      X,      S(6),   X,      X,      X,      X,      X,      X,      X,      GT(23), GT(11), GT(12), GT(13), GT(14), GT(15), GT(16), GT(17), GT(18), GT(19)  },
    {S(24), X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      S(25),  X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X       },
    {R(1),  R(1),   R(1),   R(1),   R(1),   R(1),   R(1),   R(1),   R(1),   R(1),   R(1),   R(1),   R(1),   R(1),   R(1),   R(1),   R(1),   S(26),  R(1),   R(1),   R(1),   R(1),   R(1),   R(1),   R(1),   X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X       },
    {R(4),  R(4),   R(4),   R(4),   R(4),   R(4),   R(4),   R(4),   R(4),   R(4),   R(4),   R(4),   R(4),   R(4),   R(4),   R(4),   R(4),   R(4),   R(4),   S(27),  S(28),  R(4),   R(4),   R(4),   R(4),   X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X       },
    {R(7),  R(7),   R(7),   R(7),   R(7),   R(7),   R(7),   R(7),   R(7),   R(7),   R(7),   R(7),   R(7),   R(7),   R(7),   R(7),   R(7),   R(7),   R(7),   R(7),   R(7),   S(29),  S(30),  S(31),  S(32),  X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X       },
    {R(12), R(12),  R(12),  R(12),  R(12),  S(33),  S(34),  R(12),  R(12),  R(12),  R(12),  R(12),  R(12),  R(12),  R(12),  R(12),  R(12),  R(12),  R(12),  R(12),  R(12),  R(12),  R(12),  R(12),  R(12),  X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X       },
    {R(15), S(37),  R(15),  R(15),  S(35),  R(15),  R(15),  S(36),  R(15),  R(15),  R(15),  R(15),  R(15),  R(15),  R(15),  R(15),  R(15),  R(15),  R(15),  R(15),  R(15),  R(15),  R(15),  R(15),  R(15),  X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X       },
    {R(19), R(19),  R(19),  R(19),  R(19),  R(19),  R(19),  R(19),  S(38),  R(19),  R(19),  R(19),  R(19),  R(19),  R(19),  R(19),  R(19),  R(19),  R(19),  R(19),  R(19),  R(19),  R(19),  R(19),  R(19),  X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X       },
    {R(21), R(21),  R(21),  R(21),  R(21),  R(21),  R(21),  R(21),  R(21),  R(21),  R(21),  R(21),  R(21),  R(21),  R(21),  R(21),  R(21),  R(21),  R(21),  R(21),  R(21),  R(21),  R(21),  R(21),  R(21),  X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X       },
    {R(24), R(24),  R(24),  R(24),  R(24),  R(24),  R(24),  R(24),  R(24),  R(24),  R(24),  R(24),  R(24),  R(24),  R(24),  R(24),  R(24),  R(24),  R(24),  R(24),  R(24),  R(24),  R(24),  R(24),  R(24),  X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X       },
    {R(26), R(26),  R(26),  R(26),  R(26),  R(26),  R(26),  R(26),  R(26),  R(26),  R(26),  R(26),  R(26),  R(26),  R(26),  R(26),  R(26),  R(26),  R(26),  R(26),  R(26),  R(26),  R(26),  R(26),  R(26),  X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X       },
    {R(22), R(22),  R(22),  R(22),  R(22),  R(22),  R(22),  R(22),  R(22),  R(22),  R(22),  R(22),  R(22),  R(22),  R(22),  R(22),  R(22),  R(22),  R(22),  R(22),  R(22),  R(22),  R(22),  R(22),  R(22),  X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X       },
    {R(23), R(23),  R(23),  R(23),  R(23),  R(23),  R(23),  R(23),  R(23),  R(23),  R(23),  R(23),  R(23),  R(23),  R(23),  R(23),  R(23),  R(23),  R(23),  R(23),  R(23),  R(23),  R(23),  R(23),  R(23),  X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X       },
    {X,     X,      X,      X,      X,      X,      X,      X,      X,      S(39),  X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X       },
    {X,     X,      X,      S(40),  X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      S(25),  X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X       },
    {ACCEPT, X,     X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X       },
    {X,     X,      S(9),   X,      X,      X,      S(7),   X,      X,      S(8),   X,      S(1),   S(2),   S(3),   S(4),   S(5),   X,      X,      S(6),   X,      X,      X,      X,      X,      X,      X,      X,      GT(41), GT(12), GT(13), GT(14), GT(15), GT(16), GT(17), GT(18), GT(19)  },
    {X,     X,      S(9),   X,      X,      X,      S(7),   X,      X,      S(8),   X,      S(1),   S(2),   S(3),   S(4),   S(5),   X,      X,      S(6),   X,      X,      X,      X,      X,      X,      X,      X,      X,      GT(42), GT(13), GT(14), GT(15), GT(16), GT(17), GT(18), GT(19)  },
    {X,     X,      S(9),   X,      X,      X,      S(7),   X,      X,      S(8),   X,      S(1),   S(2),   S(3),   S(4),   S(5),   X,      X,      S(6),   X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      GT(43), GT(14), GT(15), GT(16), GT(17), GT(18), GT(19)  },
    {X,     X,      S(9),   X,      X,      X,      S(7),   X,      X,      S(8),   X,      S(1),   S(2),   S(3),   S(4),   S(5),   X,      X,      S(6),   X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      GT(44), GT(14), GT(15), GT(16), GT(17), GT(18), GT(19)  },
    {X,     X,      S(9),   X,      X,      X,      S(7),   X,      X,      S(8),   X,      S(1),   S(2),   S(3),   S(4),   S(5),   X,      X,      S(6),   X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      GT(45), GT(15), GT(16), GT(17), GT(18), GT(19)  },
    {X,     X,      S(9),   X,      X,      X,      S(7),   X,      X,      S(8),   X,      S(1),   S(2),   S(3),   S(4),   S(5),   X,      X,      S(6),   X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      GT(46), GT(15), GT(16), GT(17), GT(18), GT(19)  },
    {X,     X,      S(9),   X,      X,      X,      S(7),   X,      X,      S(8),   X,      S(1),   S(2),   S(3),   S(4),   S(5),   X,      X,      S(6),   X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      GT(47), GT(15), GT(16), GT(17), GT(18), GT(19)  },
    {X,     X,      S(9),   X,      X,      X,      S(7),   X,      X,      S(8),   X,      S(1),   S(2),   S(3),   S(4),   S(5),   X,      X,      S(6),   X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      GT(48), GT(15), GT(16), GT(17), GT(18), GT(19)  },
    {X,     X,      S(9),   X,      X,      X,      S(7),   X,      X,      S(8),   X,      S(1),   S(2),   S(3),   S(4),   S(5),   X,      X,      S(6),   X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      GT(49), GT(16), GT(17), GT(18), GT(19)  },
    {X,     X,      S(9),   X,      X,      X,      S(7),   X,      X,      S(8),   X,      S(1),   S(2),   S(3),   S(4),   S(5),   X,      X,      S(6),   X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      GT(50), GT(16), GT(17), GT(18), GT(19)  },
    {X,     X,      S(9),   X,      X,      X,      S(7),   X,      X,      S(8),   X,      S(1),   S(2),   S(3),   S(4),   S(5),   X,      X,      S(6),   X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      GT(51), GT(17), GT(18), GT(19)  },
    {X,     X,      S(9),   X,      X,      X,      S(7),   X,      X,      S(8),   X,      S(1),   S(2),   S(3),   S(4),   S(5),   X,      X,      S(6),   X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      GT(52), GT(17), GT(18), GT(19)  },
    {X,     X,      S(9),   X,      X,      X,      S(7),   X,      X,      S(8),   X,      S(1),   S(2),   S(3),   S(4),   S(5),   X,      X,      S(6),   X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      GT(53), GT(17), GT(18), GT(19)  },
    {X,     X,      S(9),   X,      X,      X,      S(7),   X,      X,      S(8),   X,      S(1),   S(2),   S(3),   S(4),   S(5),   X,      X,      S(6),   X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      GT(54), GT(18), GT(19)  },
    {R(25), R(25),  R(25),  R(25),  R(25),  R(25),  R(25),  R(25),  R(25),  R(25),  R(25),  R(25),  R(25),  R(25),  R(25),  R(25),  R(25),  R(25),  R(25),  R(25),  R(25),  R(25),  R(25),  R(25),  R(25),  X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X       },
    {R(27), R(27),  R(27),  R(27),  R(27),  R(27),  R(27),  R(27),  R(27),  R(27),  R(27),  R(27),  R(27),  R(27),  R(27),  R(27),  R(27),  R(27),  R(27),  R(27),  R(27),  R(27),  R(27),  R(27),  R(27),  X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X       },
    {R(2),  R(2),   R(2),   R(2),   R(2),   R(2),   R(2),   R(2),   R(2),   R(2),   R(2),   R(2),   R(2),   R(2),   R(2),   R(2),   R(2),   S(26),  R(2),   R(2),   R(2),   R(2),   R(2),   R(2),   R(2),   X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X       },
    {R(3),  R(3),   R(3),   R(3),   R(3),   R(3),   R(3),   R(3),   R(3),   R(3),   R(3),   R(3),   R(3),   R(3),   R(3),   R(3),   R(3),   R(3),   R(3),   S(27),  S(28),  R(3),   R(3),   R(3),   R(3),   X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X       },
    {R(5),  R(5),   R(5),   R(5),   R(5),   R(5),   R(5),   R(5),   R(5),   R(5),   R(5),   R(5),   R(5),   R(5),   R(5),   R(5),   R(5),   R(5),   R(5),   R(5),   R(5),   S(29),  S(30),  S(31),  S(32),  X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X       },
    {R(6),  R(6),   R(6),   R(6),   R(6),   R(6),   R(6),   R(6),   R(6),   R(6),   R(6),   R(6),   R(6),   R(6),   R(6),   R(6),   R(6),   R(6),   R(6),   R(6),   R(6),   S(29),  S(30),  S(31),  S(32),  X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X       },
    {R(8),  R(8),   R(8),   R(8),   R(8),   S(33),  S(34),  R(8),   R(8),   R(8),   R(8),   R(8),   R(8),   R(8),   R(8),   R(8),   R(8),   R(8),   R(8),   R(8),   R(8),   R(8),   R(8),   R(8),   R(8),   X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X       },
    {R(9),  R(9),   R(9),   R(9),   R(9),   S(33),  S(34),  R(9),   R(9),   R(9),   R(9),   R(9),   R(9),   R(9),   R(9),   R(9),   R(9),   R(9),   R(9),   R(9),   R(9),   R(9),   R(9),   R(9),   R(9),   X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X       },
    {R(10), R(10),  R(10),  R(10),  R(10),  S(33),  S(34),  R(10),  R(10),  R(10),  R(10),  R(10),  R(10),  R(10),  R(10),  R(10),  R(10),  R(10),  R(10),  R(10),  R(10),  R(10),  R(10),  R(10),  R(10),  X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X       },
    {R(11), R(11),  R(11),  R(11),  R(11),  S(33),  S(34),  R(11),  R(11),  R(11),  R(11),  R(11),  R(11),  R(11),  R(11),  R(11),  R(11),  R(11),  R(11),  R(11),  R(11),  R(11),  R(11),  R(11),  R(11),  X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X       },
    {R(13), S(37),  R(13),  R(13),  S(35),  R(13),  R(13),  S(36),  R(13),  R(13),  R(13),  R(13),  R(13),  R(13),  R(13),  R(13),  R(13),  R(13),  R(13),  R(13),  R(13),  R(13),  R(13),  R(13),  R(13),  X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X       },
    {R(14), S(37),  R(14),  R(14),  S(35),  R(14),  R(14),  S(36),  R(14),  R(14),  R(14),  R(14),  R(14),  R(14),  R(14),  R(14),  R(14),  R(14),  R(14),  R(14),  R(14),  R(14),  R(14),  R(14),  R(14),  X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X       },
    {R(16), R(16),  R(16),  R(16),  R(16),  R(16),  R(16),  R(16),  S(38),  R(16),  R(16),  R(16),  R(16),  R(16),  R(16),  R(16),  R(16),  R(16),  R(16),  R(16),  R(16),  R(16),  R(16),  R(16),  R(16),  X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X       },
    {R(17), R(17),  R(17),  R(17),  R(17),  R(17),  R(17),  R(17),  S(38),  R(17),  R(17),  R(17),  R(17),  R(17),  R(17),  R(17),  R(17),  R(17),  R(17),  R(17),  R(17),  R(17),  R(17),  R(17),  R(17),  X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X       },
    {R(18), R(18),  R(18),  R(18),  R(18),  R(18),  R(18),  R(18),  S(38),  R(18),  R(18),  R(18),  R(18),  R(18),  R(18),  R(18),  R(18),  R(18),  R(18),  R(18),  R(18),  R(18),  R(18),  R(18),  R(18),  X,      X,      X,      X,      X,      X,      X,      X,      X,      X,      X       },
    {R(20),	R(20),	R(20),	R(20),	R(20),	R(20),	R(20),	R(20),	R(20),	R(20),	R(20),	R(20),	R(20),	R(20),	R(20),	R(20),	R(20),	R(20),	R(20),	R(20),	R(20),	R(20),	R(20),	R(20),	R(20),	X,      X,  	X,  	X,  	X,  	X,  	X,  	X,  	X,  	X,  	X       }
};

int index_get(token_t token) {
    switch (token)
    {
    case T_MOD:
        return P_MOD;
    case T_LPAREN:
        return P_LPAREN;
    case T_RPAREN:
        return P_RPAREN;
    case T_MUL:
        return P_MUL;
    case T_PLUS:
        return P_PLUS;
    case T_MINUS:
        return P_MINUS;
    case T_DIV:
        return P_DIV;
    case T_EXP:
        return P_EXP;
    case T_ABS:
        return P_ABS;
    case T_INT:
        return P_INT;
    case T_DBL:
        return P_DBL;
    case T_TRUE:
        return P_TRUE;
    case T_FALSE:
        return P_FALSE;
    case T_IDENT:
        return P_ID;
    case T_OR:
        return P_OR;
    case T_AND:
        return P_AND;
    case T_NOT:
        return P_NOT;
    case T_EQL_EQL:
        return P_EQL_EQL;
    case T_NOT_EQL:
        return P_NOT_EQL;
    case T_GREATER:
        return P_GREATER;
    case T_GREATER_EQL:
        return P_GREATER_EQL;
    case T_LESS:
        return P_LESS;
    case T_LESS_EQL:
        return P_LESS_EQL;
    case T_EOF:
        return P_END;
    }
}

void input_string_print(InputString input_string) {
    switch (input_string) {
    case (P_END):
        printf("P_END");
        return;
    case (P_MOD):
        printf("P_MOD");
        return;
    case (P_LPAREN):
        printf("P_LPAREN");
        return;
    case (P_RPAREN):
        printf("P_RPAREN");
        return;
    case (P_MUL):
        printf("P_MUL");
        return;
    case (P_PLUS):
        printf("P_PLUS");
        return;
    case (P_MINUS):
        printf("P_MINUS");
        return;
    case (P_DIV):
        printf("P_DIV");
        return;
    case (P_EXP):
        printf("P_EXP");
        return;
    case (P_ABS):
        printf("P_ABS");
        return;
    case (P_ERR):
        printf("P_ERR");
        return;
    case (P_INT):
        printf("P_INT");
        return;
    case (P_DBL):
        printf("P_DBL");
        return;
    case (P_TRUE):
        printf("P_TRUE");
        return;
    case (P_FALSE):
        printf("P_FALSE");
        return;
    case (P_ID):
        printf("P_ID");
        return;
    case (P_OR):
        printf("P_OR");
        return;
    case (P_AND):
        printf("P_AND");
        return;
    case (P_NOT):
        printf("P_NOT");
        return;
    case (P_EQL_EQL):
        printf("P_EQL_EQL");
        return;
    case (P_NOT_EQL):
        printf("P_NOT_EQL");
        return;
    case (P_GREATER):
        printf("P_GREATER");
        return;
    case (P_GREATER_EQL):
        printf("P_GREATER_EQL");
        return;
    case (P_LESS):
        printf("P_LESS");
        return;
    case (P_LESS_EQL):
        printf("P_LESS_EQL");
        return;
    case (P_ACCEPT):
        printf("P_ACCEPT");
        return;
    case (P_EXPR):
        printf("P_EXPR");
        return;
    case (P_CONJ):
        printf("P_CONJ");
        return;
    case (P_EQL):
        printf("P_EQL");
        return;
    case (P_REL):
        printf("P_REL");
        return;
    case (P_TERM):
        printf("P_TERM");
        return;
    case (P_FACTOR):
        printf("P_FACTOR");
        return;
    case (P_POWER):
        printf("P_POWER");
        return;
    case (P_UNARY):
        printf("P_UNARY");
        return;
    case (P_ABSVAL):
        printf("P_ABSVAL");
        return;
    case (P_LITERAL):
        printf("P_LITERAL");
        return;    
    }
}

InputString token_get(int idx) {
    return index_get(parser.token_array[idx].token_type);
}

void _stack_print(Stack *stack) {
    node_t *cur = stack->top;

    while (cur != NULL) {
        if (cur == stack->top) {
            printf("[");
            input_string_print(cur->value);
        } else {
            printf(", ");
            input_string_print(cur->value);
        }
        cur = cur->next;
        if (cur == NULL) {
            printf("]\n");
        }
    }
}

void parser_initialize(Token *token_array, int token_count) {
    parser.current = 0;
    parser.token_array = token_array;
    parser.token_count = token_count;
};

void parser_start(bool debug) {

    printf("========== STARTING SYNTAX ANALYSIS ==========\n");
    bool is_parsing = true;
    int current_state_idx = 0;
    int current_idx = 0;
    InputString current_token = token_get(current_idx);
    parsing_state current_state;

    // for state indices
    Stack *state_stack = stack_create();
    stack_push(state_stack, current_state_idx);

    // for tokens
    Stack *token_stack = stack_create();

    while(is_parsing) {
        current_state = parsing_table[current_state_idx][current_token];     // current state from parsing table (start_state - input_string -> current_state)

        // check what action to perform
        switch (current_state.action_type) {

            case SHIFT:
                if (debug) {
                    printf("q%d\t-- T%03d\t SHIFT(%d)\t--> q%d\n", current_state_idx, current_token, current_state.value, current_state.value);
                }
                stack_push(state_stack, current_state.value);
                stack_push(token_stack, current_token);
                current_state_idx = current_state.value;

                if (current_idx  < parser.token_count - 1) {
                    current_idx++;
                }

                current_token = token_get(current_idx);
                _stack_print(token_stack);
                break;
            
            case REDUCE:
                if (debug) {
                    printf("q%d\t-- T%03d\t REDUCE(%d)\t--> q%d\n", current_state_idx, current_token, current_state.value, current_state.value);
                }

                Production rule = rules[current_state.value];
                for (int i = 0; i < rule.rhs_count; i++) {
                    if (rule.rhs[rule.rhs_count-1-i] == token_stack->top->value) {
                        stack_pop(state_stack);
                        // printf("Transition: q%d -> ", stack_pop(state_stack)->value);
                        // printf("q%d\n", state_stack->top->value);
                        stack_pop(token_stack);
                    }
                }

                current_state_idx = state_stack->top->value;
                current_token = rule.lhs;
                stack_push(token_stack, current_token);
                _stack_print(token_stack);
                break;

            case GOTO:
                if (debug) {
                    printf("q%d\t-- T%03d\t GOTO(%d)\t--> q%d\n", current_state_idx, current_token, current_state.value, current_state.value);
                } 
                current_token = token_get(current_idx);
                current_state_idx = current_state.value;
                stack_push(state_stack, current_state_idx);
                break;

            case ACCEPTED:
                if (debug) {
                    printf("Accepted!\n");
                }
                is_parsing = false;
                break;

            case ERROR:
                if (debug)
                    printf("Syntax Error!\n");
                    continue;

            default:
                break;
         }

    }
    stack_free(state_stack);
    stack_free(token_stack);
};
