#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "prop.h"
#include "nnf.h"

/* CHANGE DEBUG_LEVEL IF NEEDED */

#ifndef DEBUG_LEVEL
#define DEBUG_LEVEL 0
#endif

/* DATA-STRUCTURES FOR TEST CASES */

typedef enum { PROP, NNF } test_type;

typedef struct {
        test_type type;
        union {
                Prop prop; // type = PROP
                Nnf nnf;   // type = NNF
        } u;
} test_data;

static test_data mkProp(Prop p){
        return (test_data){.type=PROP,.u={.prop=p}};
}

static test_data mkNnf(Nnf p){
        return (test_data){.type=NNF,.u={.nnf=p}};
}

typedef struct _test {
        const char *name;
        int max_var;
        int (*oracle)(set e);  /* should give the result of "eval" */
        test_data d;
} test;

typedef test (*lazy_test)();

/* BOOLEAN CONSTRUCTORS */

#define TRUE 1
#define FALSE 0

static int IMPLIES(int b1, int b2){
        return !b1 || b2;
}

/******************************/
/* ENTER YOUR PROP TESTS HERE */

static int oracle_true(set e) {
        return 1;
}
static test test_true(){
        return (test){
                .name="test_true",
                .max_var=0,
                .oracle=&oracle_true,
                .d=mkProp(true)
        };
}

static int oracle_neg_exintro(set e) {
        return !IMPLIES(TRUE && in(1,e), !in(2,e)) ;
}

static test test_neg_exintro(){
        return (test){
                .name="test_neg_exintro",
                .max_var=2,
                .oracle=&oracle_neg_exintro,
                .d=mkProp(neg(implies(and(true,var(1)),neg(var(2)))))
        };
}

static int oracle_ors(set e) {
        return !in(1,e) || in(2,e) || !in(3,e) || in(4,e);
}
static test test_ors1(){
        return (test){
                .name="test_ors1",
                .max_var=4,
                .oracle=&oracle_ors,
                .d=mkProp(or(neg(var(1)), 
                             or(var(2),
                                or(neg(var(3)),
                                   var(4)))))
        };
}
static test test_ors2(){
        return (test){
                .name="test_ors2",
                .max_var=4,
                .oracle=&oracle_ors,
                .d=mkProp(or(neg(var(1)), 
                             or(var(2),
                                or(false,
                                   or(neg(var(3)),
                                      var(4))))))
        };
}

static int oracle_neg_ors(set e) {
        return !oracle_ors(e);
}
static test test_neg_ors(){
        return (test){
                .name="test_neg_ors",
                .max_var=4,
                .oracle=&oracle_neg_ors,
                .d=mkProp(neg(or(neg(var(1)), 
                                 or(var(2),
                                    or(neg(var(3)),
                                       or(var(4),
                                          false))))))
        };
}


static int oracle_ands(set e) {
        return (in(1,e) && !in(2,e) && in(3,e));
}
static test test_ands1(){
        return (test){
                .name="test_ands1",
                .max_var=3,
                .oracle=&oracle_ands,
                .d=mkProp(and(var(1), 
                              and(neg(var(2)),
                                  var(3))))
        };
}
static test test_ands2(){
        return (test){
                .name="test_ands2",
                .max_var=3,
                .oracle=&oracle_ands,
                .d=mkProp(and(var(1),
                              and(true,
                                  and(neg(var(2)),
                                      var(3)))))
        };
}

static int oracle_neg_ands(set e) {
        return !oracle_ands(e);
}
static test test_neg_ands(){
        return (test){
                .name="test_neg_ands",
                .max_var=3,
                .oracle=&oracle_neg_ands,
                .d=mkProp(neg(and(true,
                                  and(var(1), 
                                      and(neg(var(2)),
                                          var(3))))))
        };
}

static int oracle_nmp(set e) {
        return !(IMPLIES(in(1,e),in(2,e))
                 && IMPLIES(in(2,e), in(3,e))
                 && !in(3,e));
}
static test test_nmp(){
        return (test){
                .name="test_nmp",
                .max_var=3,
                .oracle=&oracle_nmp,
                .d=mkProp(neg(and(implies(var(1),var(2)),
                                  and(implies(var(2),var(3)),
                                      neg(var(3))))))
        };
}

Prop iff(Prop p1, Prop p2){
        return and(implies(p1,p2),implies(p2,p1));
}
static test test_tauto(){
        return (test){
                .name="test_tauto",
                .max_var=2,
                .oracle=&oracle_true,
                .d=mkProp(iff(implies(var(1),var(2)),
                              implies(neg(var(2)),neg(var(1)))))
        };
}

/* DO NOT FORGET TO ADD THEM HERE ALSO ! */
#define PROP_TESTS 10
#define FIRST_NON_NNF 3
static lazy_test prop_tests [PROP_TESTS] = {
        &test_true,
        &test_ors1,
        &test_ands1,
        &test_neg_exintro,
        &test_ors2,
        &test_neg_ors,
        &test_ands2,
        &test_neg_ands,
        &test_nmp,
        &test_tauto
};



/*****************************/
/* ENTER YOUR NNF TESTS HERE */

static int oracle_false(set e) {
        return 0;
}
static test test_false1(){
        return (test){
                .name="test_false1",
                .max_var=0,
                .oracle=&oracle_false,
                .d=mkNnf(nnf_and(nnf_false(),nnf_literal(1)))
        };
}
static test test_false2(){
        return (test){
                .name="test_false2",
                .max_var=0,
                .oracle=&oracle_false,
                .d=mkNnf(nnf_and(nnf_literal(-2), nnf_false()))
        };
}
static test test_true1(){
        return (test){
                .name="test_true1",
                .max_var=0,
                .oracle=&oracle_true,
                .d=mkNnf(nnf_or(nnf_true(),nnf_literal(-3)))
        };
}
static test test_true2(){
        return (test){
                .name="test_false2",
                .max_var=0,
                .oracle=&oracle_true,
                .d=mkNnf(nnf_or(nnf_literal(4), nnf_true()))
        };
}

static int oracle_one(set e) {
        return in(1,e);
}
static test test_one1(){
        return (test){
                .name="test_one1",
                .max_var=1,
                .oracle=&oracle_one,
                .d=mkNnf(nnf_and(nnf_true(),nnf_literal(1)))
        };
}
static test test_one2(){
        return (test){
                .name="test_one2",
                .max_var=1,
                .oracle=&oracle_one,
                .d=mkNnf(nnf_and(nnf_literal(1), nnf_true()))
        };
}
static test test_one3(){
        return (test){
                .name="test_one3",
                .max_var=1,
                .oracle=&oracle_one,
                .d=mkNnf(nnf_or(nnf_false(),nnf_literal(1)))
        };
}
static test test_one4(){
        return (test){
                .name="test_one4",
                .max_var=1,
                .oracle=&oracle_one,
                .d=mkNnf(nnf_or(nnf_literal(1), nnf_false()))
        };
}

static test test_nnf_ors(){
        return (test){
                .name="test_nnf_ors",
                .max_var=4,
                .oracle=&oracle_ors,
                .d=mkNnf(nnf_or(nnf_literal(-1), 
                                nnf_or(nnf_literal(2),
                                       nnf_or(nnf_literal(-3),
                                              nnf_or(nnf_literal(4),
                                                     nnf_false())))))
        };
}

static test test_nnf_ands(){
        return (test){
                .name="test_nnf_ands",
                .max_var=3,
                .oracle=&oracle_ands,
                .d=mkNnf(nnf_and(nnf_true(),
                                 nnf_and(nnf_literal(1), 
                                         nnf_and(nnf_literal(-2),
                                                 nnf_literal(3)))))
        };
}


/* DO NOT FORGET TO ADD THEM HERE ALSO ! */
#define NNF_TESTS 10
static lazy_test nnf_tests [NNF_TESTS] = {
        &test_false1,
        &test_false2,
        &test_true1,
        &test_true2,
        &test_one1,
        &test_one2,
        &test_one3,
        &test_one4,
        &test_nnf_ors,
        &test_nnf_ands
};

/********************************/
/* AUXILIARY FUNCTION FOR TESTS */

static int (*oracle)(set e);
static Prop p_under_test;
static int warning=0;

static void test_equiv(set e){
        int r1, r2;
        #if DEBUG_LEVEL >= 1
        printf(" eval in ");
        print(e);
        #endif
        r1=eval(p_under_test, e);
        #if DEBUG_LEVEL >= 2
        printf(" found %d\n", r1);
        #endif
        r2=(*oracle)(e);
        if (r1 != r2) {
                printf(" ERROR found on eval in environment:");
                print(e);
                printf(" Found %d vs %d expected\n", r1, r2);
                if (warning) {
                        printf(" Please, check if the proposition (see infix notation) is correct...\n");
                        printf(" See how it is built in tests.c (using the name of the test).\n");
                }
                exit(1);
        }
}

static void equiv(test t, Prop p){
        oracle=t.oracle;
        p_under_test=p;
        enumall(t.max_var, &test_equiv);
}

static Prop access_Prop(test t){
        switch (t.d.type){
        case PROP:
                return t.d.u.prop;
        default:
                return to_Prop(t.d.u.nnf);
        }
}

static void basic(lazy_test input){
        test t;
        int mv;
        Prop p;
        printf("--- new basic test\n");
        t=(*input)();
        printf("  name: %s\n", t.name);
        p=access_Prop(t);
        printf("  infix: ");
        print_infix(p);
        mv=max_var(p);
        if (mv != t.max_var) {
                printf("  ERROR max_var: found %d versus %d expected\n", mv, t.max_var);
                exit(1);
        }
        printf("  max_var: %d\n", mv);        
        if (t.d.type == NNF) {
                assert(is_Nnf(p));
        }
        equiv(t, p);
}

static void check_isNnf(lazy_test input, int expected){
        test t;
        Prop p;
        int r;
        printf("--- new isNnf test\n");
        t=(*input)();
        printf("  name: %s\n", t.name);
        p=access_Prop(t);
        printf("  infix: ");
        print_infix(p);
        r=is_Nnf(p);
        if (is_Nnf(p) != expected) {
                printf("  ERROR is_Nnf: found %d versus %d expected\n", r, expected);
                exit(1);
        }
}

static void loop(lazy_test input){
        test t;
        Prop p;
        printf("--- new loop test\n");
        t=(*input)();
        printf("  name: %s\n", t.name);
        p=to_Prop(to_Nnf(access_Prop(t)));
        printf("  infix nnf: ");
        print_infix(p);
        assert(is_Nnf(p));
        equiv(t, p);
}


int main(int argc, char *argv[]){
        int i;

        #if DEBUG_LEVEL >= 10
        // SI BESOIN, AJOUTER DU CODE DE DEBOGAGE CI-DESSOUS
        printf("DEBUT Code de débogage !\n");
       
        // Un exit ci-dessous pour éviter de faire le reste des tests !
        printf("FIN Code de débogage: si OK, remettre DEBUG_LEVEL à 0 !\n");
        exit(1);
        #endif
                
        for (i=0; i<PROP_TESTS; i++){
                basic(prop_tests[i]);
        }
        printf("### %d BASIC PROP TESTS PASSED\n\n", PROP_TESTS); 

        for (i=0; i<PROP_TESTS; i++){
                check_isNnf(prop_tests[i], i<FIRST_NON_NNF);
        }
        printf("### %d check_isNnf TESTS PASSED\n\n", PROP_TESTS); 

        warning=1;
        for (i=0; i<NNF_TESTS; i++){
                basic(nnf_tests[i]);
        }
        printf("### %d BASIC NNF TESTS PASSED\n\n", NNF_TESTS);

        for (i=0; i<PROP_TESTS; i++){
                loop(prop_tests[i]);
        }
        printf("### %d LOOP PROP TESTS PASSED\n\n", PROP_TESTS); 

        for (i=0; i<NNF_TESTS; i++){
                loop(nnf_tests[i]);
        }
        printf("### %d LOOP NNF TESTS PASSED\n\n", NNF_TESTS);

        printf("### ALL PASSED\n");
        return 0;
}

        
