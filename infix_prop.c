/* Implementation of a calculator in infix notation */

#include <stdio.h>
#include <stdlib.h>
#include "lexer.h"
#include "cmdline.h"
#include "prop.h"

/* CHANGE DEBUG_LEVEL IF NEEDED */

#ifndef DEBUG_LEVEL
#define DEBUG_LEVEL 0
#endif

/* look a-head */
static token current;
static int att; /* attribute of current */ 

/* parsing of token */
static void parse_token(token expected) {
        if (current != expected) {
                printf("ERROR line %d col %d: expected token ", numl, numc-1);
                display(expected, -1);
                printf(" but found ");
                display(current, att);
                printf("\n");
                exit(1);
        }
        if (current != END) {
                current = next(&att);
                #if DEBUG_LEVEL > 0
                printf("# DEBUG: next token ");
                display(current, att);
                printf("\n");
                #endif        
        }
}

/* A COMPLETER */


static Prop parse() {
        Prop p;
        /* init current and att */
        current = next(&att);
        /* A CHANGER */ 
        printf("parse: VERSION FOURNIE A CORRIGER !\n");
        p = true;
        parse_token(END);
        return p;
}
/*
static Prop NT_P0(Prop prop) {
  switch(current){
  case '(':
    NT_P3(prop);
  }

*/


Prop parse_p0(Prop* p)
{

   switch (current) {

  case PAR1:
    parse_token(PAR1);
    parse_p3(p);
    parse_token(PAR2);
    return p;

  case VAR:
    *p = var(att);
    parse_token(VAR);

  case TRUE:
    *p = true;
    parse_token(VAR);

  case FALSE:
    *p = false;
    parse_token(VAR);

  case NEG:
    *Prop tmp;
    parse_token(NEG);
    parse_p0(p);
    
    
    parse_p3(tmp);
    *p;


  }
  return p;
}

Prop parse_p1(Prop p)
{
  unexpected(current, att, "while beginning a proposition");
  return p;
}

Prop parse_p2(Prop p)
{
  return p;
}

Prop

Prop parse_p3x(Prop p)
{
  switch (current) {

  case END:
  case '(':
    return p;

  case IMPLIES:

    parse_p3(p);
    return p;


  }
  return p;
  
}

int main(int argc, char *argv[]) {
        run(&parse, "infix", argc, argv);
        return 0;
}
