/* Interface of a lexer */

#ifndef _LEXER_H
#define _LEXER_H 1

typedef enum { VAR, TRUE, FALSE, NEG, AND, OR, IMPLIES, END } token;

/* next(v) returns the next token of the input stream.
   *v is only modified when the returned token is VAR
   In this case, *v is the positive integer read in the stream.
*/ 
extern token next(int *v);

/* display t and its possible attribute v
   if v <= 0 then v is considered as irrelevant 
   (it is printed as "0")
*/
extern void display(token t, int v);

/* print an error message and stop execution (exit)
   if (mesg[0] == 0) it is ignored,
   otherwise, it gives an hint 
 */
extern void unexpected(token t, int v, const char *mesg);

/* position in the input stream of the next token to read 
   (following emacs convention, line starts at 1 and column at 0). 
*/
extern int numl;
extern int numc;

#endif /* !_LEXER_H */
