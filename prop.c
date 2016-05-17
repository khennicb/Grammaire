#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "prop.h"
#include "nnf.h"

//
// definition of type Prop

struct Prop_Node {
        Prop_Root root;
        union {
                int id;   // case N_var
                Prop son; // case N_neg
                struct { Prop sonL, sonR; } bin; // case N_and, N_or, N_implies
        } u;
};

//
// definition of constant constructors
static struct Prop_Node _true = {N_true};
Prop const true = &_true;

static struct Prop_Node _false = {N_false};
Prop const false = &_false;

//
// definition of non-constant constructors
static Prop newProp(struct Prop_Node x) {
        struct Prop_Node *res = malloc(sizeof(struct Prop_Node));
        if (res==NULL) {
                printf("ERROR: unexpected null pointer. No memory left ?\n");
                exit(1);
        }
        *res = x;
        return res;
}

Prop var(int x) {
        assert (x > 0);
        return newProp((struct Prop_Node){N_var,{.id=x}});
}

Prop neg(Prop son) {
        return newProp((struct Prop_Node){N_neg,{.son=son}});
}

Prop and(Prop sonL, Prop sonR) {
        return newProp((struct Prop_Node){N_and,{.bin={sonL,sonR}}});
}

Prop or(Prop sonL, Prop sonR) {
        return newProp((struct Prop_Node){N_or,{.bin={sonL,sonR}}});
}

Prop implies(Prop sonL, Prop sonR) {
        return newProp((struct Prop_Node){N_implies,{.bin={sonL,sonR}}});
}

/***********/
/* max_var */

static int max(int v1, int v2){
        return (v1>v2?v1:v2);
}

int max_var(Prop p){
        switch (p->root) {
        case N_true: case N_false:
                return 0;
        case N_var:
                return p->u.id;
        case N_neg:
                return max_var(p->u.son);
        case N_and: case N_or: case N_implies:
                return max(max_var(p->u.bin.sonL), max_var(p->u.bin.sonR));
        default:
                // This case should be impossible
                assert (1!=1);
                return 0;
        }
}

/********/
/* eval */

int eval(Prop p, set env){
        switch (p->root) {
        case N_var:
                return in(p->u.id, env);
	case N_true:
		return 1;
	case N_false:
		return 0;
	case N_neg:
	  return (eval(p->u.son,env)+1)%2;
	case N_and:
	  if(eval(p->u.bin.sonL,env)==1 && eval(p->u.bin.sonR,env)==1 ){
			return 1;
		}
		else {
			return 0;
		}
	case N_or:
	  if(eval(p->u.bin.sonL, env)==1 || eval(p->u.bin.sonR,env)==1 ){
			return 1;
		}
		else {
			return 0;
		}
	case N_implies:
	  if(eval(p->u.bin.sonL, env)==0 || eval(p->u.bin.sonR,env)==1 ){
			return 1;
		}
		else {
			return 0;
		}

        default:
                // This case should be impossible
                assert (1!=1);
                return 0;
        }
}

/**********/
/* is_Nnf */

/* decide if p is a non-constant Nnf */
static int is_Ncst(Prop p);


static int is_Ncst(Prop p){
        switch (p->root) {

	case N_var:
	  return 1;
	case N_neg:
	  return p->u.son->root == N_var;
	case N_and: case N_or:
	  return (is_Ncst(p->u.bin.sonL) &&
		  is_Ncst(p->u.bin.sonR));
	  case N_true: case N_false: case N_implies:
                return 0;
        default:
                return 0;
        }
}
/* decide if p is a Nnf */
int is_Nnf(Prop p){
        switch (p->root) {
	case N_true: case N_false:
                return 1;

        default:
	  return is_Ncst(p);
        }
}



/**********/
/* to_Nnf */

static Nnf to_Nnfx(Prop p, int has_neg);

Nnf to_Nnf(Prop p){
        return to_Nnfx(p, 0);
}

static Nnf to_Nnfx(Prop p, int ng) {
        switch (p->root) {

        case N_true:
          if(!ng){ // si positif
                return nnf_true();
          }else {  // si négatif
                return nnf_false();
          }

        case N_false:
          if(!ng){ // si positif
                return nnf_false();
          }else {  // si négatif
                return nnf_true();
          }

        case N_var:
          if(!ng){ // si positif
            return nnf_literal(p->u.id);
          }else {  // si négatif
            return nnf_literal(-p->u.id);
          }
        case N_neg:
                return to_Nnfx(p->u.son, !ng);
        case N_and:
            if(!ng) // si positif
                return nnf_and(to_Nnfx(p->u.bin.sonL, ng), to_Nnfx(p->u.bin.sonR, ng));
            else    // si négatif
                return nnf_or(to_Nnfx(p->u.bin.sonL, ng), to_Nnfx(p->u.bin.sonR, ng));
        case N_or:
            if(!ng) // si positif
                return nnf_or(to_Nnfx(p->u.bin.sonL, ng), to_Nnfx(p->u.bin.sonR, ng));
            else    // si négatif
                return nnf_and(to_Nnfx(p->u.bin.sonL, ng), to_Nnfx(p->u.bin.sonR, ng));

        case N_implies:
            if(!ng) // si positif
                return nnf_or(to_Nnfx(p->u.bin.sonL,!ng),to_Nnfx(p->u.bin.sonR,ng));
            else    // si négatif
                return nnf_and(to_Nnfx(p->u.bin.sonL,!ng),to_Nnfx(p->u.bin.sonR,ng));


        default:
                // This case should be impossible
                assert (1!=1);
                return NULL;
        }
}


/*******************/
/* Prefix printing */

static void print_prefix_indent(int level, Prop p);

void print_prefix(Prop p) {
        print_prefix_indent(1,p);
}

static void print_prefix_indent(int level, Prop p){
        switch (p->root) {
        case N_true:
                printf("%*c\n",level,'t');
                break;
        case N_false:
                printf("%*c\n",level,'f');
                break;
        case N_var:
                printf("%*c%d\n",level-1,' ',p->u.id);
                break;
        case N_neg:
                printf("%*c\n",level,'-');
                print_prefix_indent(level+1,p->u.son);
                break;
        case N_and:
                printf("%*c\n",level,'&');
                print_prefix_indent(level+1,p->u.bin.sonL);
                print_prefix_indent(level+1,p->u.bin.sonR);
                break;
        case N_or:
                printf("%*c\n",level,'|');
                print_prefix_indent(level+1,p->u.bin.sonL);
                print_prefix_indent(level+1,p->u.bin.sonR);
                break;
        case N_implies:
                printf("%*c\n",level,'>');
                print_prefix_indent(level+1,p->u.bin.sonL);
                print_prefix_indent(level+1,p->u.bin.sonR);
                break;
        default:
                // This case should be impossible
                assert (1!=1);
        }
}


/******************/
/* Infix printing */

static void print_infix_precedence(int level, Prop p);

void print_infix(Prop p) {
        print_infix_precedence(3,p);
        printf("\n");
}

static void parenth(int father, int curr, char c) {
        if (father < curr) {
                printf("%c",c);
        }
}

static void print_infix_op(int father, int curr, int currL, char op, Prop sonL, Prop sonR) {
        parenth(father,curr,'(');
        print_infix_precedence(currL,sonL);
        printf("%c",op);
        print_infix_precedence(curr,sonR);
        parenth(father,curr,')');
}

static void print_infix_precedence(int level, Prop p) {
        switch (p->root) {
        case N_true:
                printf("t");
                break;
        case N_false:
                printf("f");
                break;
        case N_var:
                printf("%d",p->u.id);
                break;
        case N_neg:
                printf("-");
                print_infix_precedence(0,p->u.son);
                break;
        case N_and:
                print_infix_op(level,1,1,'&',p->u.bin.sonL,p->u.bin.sonR);
                break;
        case N_or:
                print_infix_op(level,2,2,'|',p->u.bin.sonL,p->u.bin.sonR);
                break;
        case N_implies:
                print_infix_op(level,3,2,'>',p->u.bin.sonL,p->u.bin.sonR);
                break;
        default:
                // This case should be impossible
                assert (1!=1);
        }
}

// MISC
Prop_Root root(Prop p){
        assert (p != NULL);
        return p->root;
}
