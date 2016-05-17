#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

/* We skip definition of Nnf */
#define _SORTS_SKIP_Nnf 1
#include "sorts.h"

/* Here we identify Nnf and Prop */
typedef Prop Nnf;

#include "nnf.h"
#include "prop.h"

//
// definition of constant constructors

Nnf nnf_true(){
        return true;
}

Nnf nnf_false(){
        return false;
}

// Smart constructors
Nnf nnf_literal(int x) {
        assert (x != 0);
	if (x<0) {
	  Nnf literal = (Nnf) var(-x);
	  return neg(literal);
	} else {
	  return var(x);
	}
}

Nnf nnf_and(Nnf sonL, Nnf sonR) {

  switch(root(sonL)) {
      case N_true:
	return sonR;
      case N_false:
	return false;
      default:
	switch(root(sonR)) {
	      case N_true:
		return sonL;
	      case N_false:
		return false;
	      default:
		return and(sonL, sonR);
      }

  }
}

Nnf nnf_or(Nnf sonL, Nnf sonR) {

  switch(root(sonL)) {
      case N_true:
	return true;
      case N_false:
	return sonR;
      default:
	switch(root(sonR)) {
	      case N_true:
		return true;
	      case N_false:
		return sonL;
	      default:
		return or(sonL, sonR);
      }
  }
}

/* Translation */

Prop to_Prop(Nnf p) {
        return p;
}
