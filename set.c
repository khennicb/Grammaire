/* A simple implementation of "set.h" interface
*/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "set.h"

#ifndef DEBUG_LEVEL
#define DEBUG_LEVEL 0
#endif

struct _set {int size; char *elements;};

int in(int v, set e){
        assert (e != NULL && v > 0);
        if (v <= e->size) {
                return e->elements[v-1];
        }
        return 0;
}

void print(set e){
        int v;
        assert (e != NULL);
        for (v=0; v < e->size; v++) {
                if (e->elements[v]) 
                        printf("%d ",v+1);
        }
        printf("\n");
}

/* protect from NULL pointer returned by malloc or realloc */
static void *protect(void *p){
        if (p==NULL) {
                printf("ERROR: unexpected null pointer. No memory left ?\n");
                exit(1);
        }
        return p;
}

/* return an empty set */
static set empty() {
        set res=protect(malloc(sizeof(struct _set)));
        res->size=0;
        res->elements=NULL;
        return res;
}

/* update e as e' such that
     e' = e - {v}      if in(e,v)
     e' = e union {v}  otherwise
*/
static void xor(set e, int v) {
        int i;
        assert(e != NULL && 1 <= v);
        if (v > e->size) {
                if (e->elements == NULL) {
                        e->elements=protect(malloc(v));
                } else {
                        e->elements=protect(realloc(e->elements,v));
                }
                for (i=e->size; i<v; i++) {
                        e->elements[i]=0;
                }
                e->size=v;
        }
        e->elements[v-1] = !e->elements[v-1];
        #if DEBUG_LEVEL >= 1
        printf("%d := %d\n", v, e->elements[v-1]);
        #endif
}

set from_array(int size, int present[]){
        int i, max;
        set res;
        if (size <= 0) {
                return empty(size);
        }
        max = present[0];
        for (i = 1; i < size; i++) {
                if (present[i] > max) {
                        max = present[i];
                }
        }
        res=empty();
        xor(res, max);
        for (i = 0; i<size; i++){
                res->elements[present[i]-1] = 1;
        }
        return res;
}

static void destruct(set *e){
        set x;
        assert (e != NULL);
        x=*e;
        *e=NULL;
        if (x == NULL) 
                return;
        free(x->elements);
        free(x);
}

static void rec_enum(int v, set e, void (*f)(set)){
        if (v<=0) {
                (*f)(e);
                return;
        }
        xor(e,v); // in principle, assigns v=1
        rec_enum(v-1,e,f);
        xor(e,v); // in principle, assigns v=0
        rec_enum(v-1,e,f);
}

void enumall(int v, void (*f)(set)){
        set e=empty();
        rec_enum(v, e, f);
        destruct(&e);
}
