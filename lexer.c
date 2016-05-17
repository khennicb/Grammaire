/* Implementation of "lexer.h" */

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "lexer.h"

/* NB: allocation below requires that END is the last token
   TAKE CARE to synchronize token_name with token type of lexer.h
*/
static const char* token_name[END+1] = {
        "",
        "t",
        "f",
        "-",
        "&",
        "|",
        ">",
        "<EOF>"
};

void display(token t, int v) {
        printf("%s", token_name[t]);
        switch (t) {
        case VAR:
                if (v > 0) {
                        printf("%d",v);
                } else {
                        printf("0");
                }
        default:
                break;
        }
}

void unexpected(token t, int v, const char *mesg) {
        printf("ERROR line %d col %d: unexpected token ", numl, numc-1);
        display(t, v);
        if (mesg[0] != 0) {
                printf(" %s", mesg);
        }
        printf("\n");
        exit(1);
}


static int isDigit(char c) {
        return '0' <= c && c <= '9';
}

static int digit(char c) {
        assert(isDigit(c));
        return c - '0';
}

int numl = 0;
int numc = 0;
static char current;

static void update_current() {
        current = fgetc(stdin);
        numc++;
}

static void parse_int(int *v) {
       int res; 
        if (! isDigit(current) || current == '0') {
                printf("LEXICAL ERROR line %d col %d: unexpected character '%c'\n", numl, numc, current);
                exit(1);
        }        
        res = digit(current);
        update_current();
        /* A COMPLETER et CORRIGER: LECTURE DU +LONG ENTIER POSSIBLE ! */
        
        while(current!=' ' && current!='\t' && current!='\n'){
            if (! isDigit(current)) {
                printf("LEXICAL ERROR line %d col %d: unexpected character '%c'\n", numl, numc, current);
                exit(1);
            }  
            res = res * 10;
            res += digit(current);
            update_current();
        }
        *v = res;
}

static void skip_spaces() {
        int insideComments = 0;
        do {
                switch (current) {
                case ' ': case '\t':
                        break;
                case '\n':
                        numc = 0;
                        numl++;
                        insideComments = 0;
                        break;
                case '#':
                        insideComments = 1;
                        break;
                case EOF:
                        return;
                default:
                        if (insideComments) {
                                break;
                        } else {
                                return;
                        }
                }
                update_current();
        } while(1);
}

token next(int *v) {
        static int init = 0;
        if (! init) {
                update_current();
                init = 1;
                numl = 1;
        }
        skip_spaces();
        switch (current) {
        case 't':
            update_current();
            return TRUE;
        case 'f':
            update_current();
            return FALSE;
        case '-':
            update_current();
            return NEG;
        case '&':
            update_current();
            return AND;
        case '|':
            update_current();
            return OR;
        case '>':
            update_current();
            return IMPLIES;
        case EOF:
                return END;
        default:
                parse_int(v);
                return VAR;
        }
}
