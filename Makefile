##########
# MAKEFILE FOURNI
#
# En principe: inutile de le modifier !

GCC := gcc -g -Wall -Werror
EXE := debug_lexer tests prefix_prop infix_prop

.PHONY: exe execlean

exe: $(EXE)

# executables
tests: tests.o prop.o nnf.o set.o
	$(GCC) $^ -o $@

debug_lexer: debug_lexer.o lexer.o
	$(GCC) $^ -o $@

prefix_prop: prefix_prop.o lexer.o cmdline.o prop.o nnf.o set.o
	$(GCC) $^ -o $@

infix_prop: infix_prop.o lexer.o cmdline.o prop.o nnf.o set.o
	$(GCC) $^ -o $@

# dependencies
set.o: set.c set.h
prop.o: prop.c prop.h nnf.h sorts.h set.h
nnf.o: nnf.c nnf.h prop.h sorts.h
tests.o: tests.c prop.h nnf.h sorts.h set.h 
lexer.o: lexer.c lexer.h
debug_lexer.o: debug_lexer.c lexer.h
cmdline.o: cmdline.c cmdline.h sorts.h prop.h nnf.h set.h
prefix_prop.o: prefix_prop.c lexer.h cmdline.h sorts.h prop.h
infix_prop.o: infix_prop.c lexer.h cmdline.h sorts.h prop.h

# modular compilation
%.o: %.c
	$(GCC) -c $< -o $@

# cleaning
execlean:
	$(RM) $(EXE) *.o *~
