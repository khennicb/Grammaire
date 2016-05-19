#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "cmdline.h"
#include "prop.h"
#include "nnf.h"

/*****************************/
/* treatment on propositions */

static void _eval(parser proc, int argc, char *argv[]){
        int arg[argc];
        int i=0;
        set env;
        Prop p;
        for (i=0; i < argc; i++) {
                arg[i]=atoi(argv[i]);
                if (arg[i]<=0) {
                        printf(" ERROR: %d-th argument %s is not a valid variable name\n", i, argv[i]);
                        printf(" Variable must be positive integers\n");
                        exit(1);
                }
        }
        env=from_array(argc, arg);
        p=(*proc)();
        printf("# eval in ");
        print(env);
        printf("# result = %s\n",(eval(p,env)?"TRUE":"FALSE"));
}

static void _print_prefix(parser proc, int argc, char *argv[]){
        print_prefix((*proc)());
}
static void _print_nnf_prefix(parser proc, int argc, char *argv[]){
        print_prefix(to_Prop(to_Nnf((*proc)())));
}
static void _print_infix(parser proc, int argc, char *argv[]){
        print_infix((*proc)());
}
static void _print_nnf_infix(parser proc, int argc, char *argv[]){
        print_infix(to_Prop(to_Nnf((*proc)())));
}

static Prop p_under_test;

static void test_tauto(set e){
        if (!eval(p_under_test, e)) {
                printf("# FALSE in environment: ");
                print(e);
                exit(1);
        }
}

static void tauto(parser proc, int argc, char *argv[]){
       p_under_test  = (*proc)();
       enumall(max_var(p_under_test), &test_tauto);
       printf("# TAUTOLOGY !\n");
}

/***********************************************/
/* OPTIONS IN INCREASING LEXICOGRAPHIC ORDER ! */
#define NUM_OPTION 6
static struct {
        char *name;
        char *desc;
        void (*action)(parser proc, int argc, char *argv[]);
        int noarg;
} option[NUM_OPTION] = {
        { .name = "-e", .desc = "eval in an environment (a set of variables) read from arguments",  .action = &_eval, .noarg=0 },
        { .name = "-i", .desc = "outputs Prop in infix format",  .action = &_print_infix, .noarg=1 },
        { .name = "-in", .desc = "outputs Nnf in infix format",  .action = &_print_nnf_infix, .noarg=1 },
        { .name = "-p", .desc = "outputs Prop in prefix format",  .action = &_print_prefix, .noarg=1 },
        { .name = "-pn", .desc = "outputs Nnf in prefix format",  .action = &_print_nnf_prefix, .noarg=1 },
        { .name = "-t", .desc = "check that Prop is a tautology with a very naive algorithm",  .action = &tauto, .noarg=1 },
};

/* dichotomic search */
static int find_option(char *arg){
        int l=0, r=NUM_OPTION-1, m, x;
        while (l <= r) {
                m = (l + r) / 2;
                x = strcmp(arg, option[m].name);
                if (x == 0) {
                        return m;
                }
                if (x < 0) {
                        r = m-1;
                } else {
                        l = m+1;
                }
        }
        return -1;
}


static void help(const char *parser_name, char *prog_name){
        int i;
        printf("%s mandatory-option [arguments]\n", prog_name);
        printf("  parse a Prop in %s format on standard input\n", parser_name);
        printf("  and apply on this Prop the procedure described by mandatory-option\n\n");
        printf("mandatory option:\n");
        for (i=0; i < NUM_OPTION; i++) {
                printf("  %s\t %s %s\n", option[i].name, option[i].desc, (option[i].noarg?"(rejects any argument)":""));
        }
}


void run(parser proc, const char *parser_name, int argc, char *argv[]){
        int i;
        assert(argc >= 1);
        if (argc == 1) {
                printf("ERROR: expect a mandatory option on the command line (see below)\n\n");
                help(parser_name, argv[0]);
                exit(1);
        }
        i = find_option(argv[1]);
        if (i < 0){
                printf("ERROR: unknown option %s (see below)\n\n", argv[1]);
                help(parser_name, argv[0]);
                exit(1);
        }
        if (option[i].noarg && argc > 2) {
                printf("ERROR: option %s does not expect arguments (see below)\n\n", argv[1]);
                help(parser_name, argv[0]);
                exit(1);
        }
        (*option[i].action)(proc, argc-2, argv+2);
}
