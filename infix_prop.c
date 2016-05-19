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

int main(int argc, char *argv[]) {
        run(&parse, "infix", argc, argv);
        return 0;
}
