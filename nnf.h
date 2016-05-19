#ifndef _NNF_H
#define _NNF_H 1

#include "sorts.h"

/* Smart constructors */
extern Nnf nnf_true();
extern Nnf nnf_false();
extern Nnf nnf_literal(int);
extern Nnf nnf_and(Nnf, Nnf);
extern Nnf nnf_or(Nnf, Nnf);

/* Translation */
extern Prop to_Prop(Nnf);

#endif
