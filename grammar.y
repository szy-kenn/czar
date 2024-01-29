%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
%}

%union {
  char *str;
}

%token <str> IDENT STR CHR BOOL NIL INT DBL INDENT DEDENT

%token GLOBAL FIXED FLEX
%token INPUT OUTPUT WHEN ELSE LOOP
%token AND OR EQUALS NOTEQUALS GREATER GREATEREQUAL LESS LESSEQUAL PLUS MINUS TIMES DIVIDE MODULO POW NOT ABS
%token COLON EQUAL

%left OR
%left AND
%left EQUALS NOTEQUALS
%left GREATER GREATEREQUAL LESS LESSEQUAL
%left PLUS MINUS
%left TIMES DIVIDE MODULO
%right POW NOT ABS

%type <str> program stmts stmt decl_stmt mut_type assign_stmt value input_stmt
%type <str> output_stmt string_consts string_const when_stmt else_stmt loop_stmt
%type <str> data_type expression conjunction equality relational term factor power unary abs literal

%%

program: stmts;

stmts: stmts stmt
     | stmt;

stmt: decl_stmt
    | assign_stmt
    | input_stmt
    | output_stmt
    | when_stmt
    | loop_stmt;

decl_stmt: GLOBAL mut_type IDENT COLON data_type
         | GLOBAL mut_type IDENT COLON EQUAL value
         | mut_type IDENT COLON data_type
         | mut_type IDENT COLON data_type EQUAL value;

mut_type: FIXED
        | FLEX;

assign_stmt: IDENT EQUAL value;

value: input_stmt
     | expression
     | STR
     | CHR
     | BOOL
     | NIL;

input_stmt: INPUT "(" string_consts ")"
          | INPUT "(" ")";

output_stmt: OUTPUT "(" string_consts ")"
           | OUTPUT "(" ")";

string_consts: string_consts "&" string_const
             | string_const;

string_const: expression
            | STR
            | CHR;

when_stmt: WHEN expression COLON INDENT stmt DEDENT else_stmt;

else_stmt: ELSE WHEN expression COLON INDENT stmt DEDENT else_stmt
         | ELSE COLON INDENT stmt DEDENT
         | /* empty */;

loop_stmt: LOOP IDENT "in" "(" expression ")" COLON INDENT stmt DEDENT
         | LOOP IDENT "in" "(" expression "to" expression ")" COLON INDENT stmt DEDENT
         | LOOP IDENT "in" "(" expression "to" expression "by" expression ")" COLON INDENT stmt DEDENT;

data_type: "int"
         | "dbl"
         | "str"
         | "chr"
         | "bool"
         | "nil";

expression: expression OR conjunction
          | conjunction;

conjunction: conjunction AND equality
           | equality;

equality: equality EQUALS relational
        | equality NOTEQUALS relational
        | relational;

relational: relational GREATER abs
          | relational GREATEREQUAL abs
          | relational LESS abs
          | relational LESSEQUAL abs
          | abs;

abs: ABS abs ABS
   | term;

term: term PLUS factor
    | term MINUS factor
    | factor;

factor: factor TIMES power
      | factor DIVIDE power
      | factor MODULO power
      | power;

power: power POW unary
     | unary;

unary: NOT unary
     | MINUS unary
     | literal;

literal: "(" expression ")"
       | INT
       | DBL
       | "true"
       | "false"
       | IDENT;

%%

int main() {
  yyparse();
  return 0;
}

int yyerror(const char *s) {
  fprintf(stderr, "Error: %s\n", s);
  return 0;
}
