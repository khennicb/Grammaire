/* Implementation of a parser in prefix notation */

#include <stdio.h>
#include "lexer.h"
#include "cmdline.h"
#include "prop.h"

static void parse_rec(Prop *p) {

        // Je n'ai pas fini cette partie.

        int v;
        token current = next(&v);

        switch (current) {
/*        case VAR:
                *p = var(1); // TODO : chercher comment récuperer le nom de la variable ??
                break;
        case TRUE:
                *p = true;
                break;
        case FALSE:
                *p = true;
                break;
        case NEG:
            parse_rec( &(*p)->u.son)
                *p = neg(p);
                break;
        case AND:
                *p = and(parse_rec(p->u.sonL),parse_rec(p->u.sonR));
                break;
        case OR:
                *p = or(parse_rec(p->u.sonL),parse_rec(p->u.sonR));
                break;
        case IMPLIES:
                *p = implies(parse_rec(p->u.sonL),parse_rec(p->u.sonR));
                break;
        case END:
                break;
        */default:
                unexpected(current, v, "in a proposition");
        }
}

static Prop parse() {
        int v;
        token current;
        Prop p;
        parse_rec(&p);
        current = next(&v);
        if (current != END){
                unexpected(current, v, "after the proposition");
        }
        return p;
}

int main(int argc, char *argv[]) {
        run(&parse, "prefix", argc, argv);
        return 0;
}


