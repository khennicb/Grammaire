#ifndef _PROP_H
#define _PROP_H 1

#include "sorts.h"
#include "set.h"

/* Constructors */
extern Prop const true;
extern Prop const false;
extern Prop var(int);
extern Prop neg(Prop);
extern Prop and(Prop, Prop);
extern Prop or(Prop, Prop);
extern Prop implies(Prop, Prop);

/* Interpreters */
extern int max_var(Prop);
extern int eval(Prop, set);
extern int is_Nnf(Prop);
extern Nnf to_Nnf(Prop);

/* Outputs */
extern void print_prefix(Prop);
extern void print_infix(Prop);

/* Partial access to internal representation for nnf.c */ 

typedef enum { N_true, N_false, 
               N_var, N_neg, N_and, N_or, N_implies } Prop_Root;

extern Prop_Root root(Prop);


#endif
