/* An abstract datatype for set of "variables" (ie positive integers).
*/

#ifndef _SET_H
#define _SET_H 1

typedef struct _set *set;

/* v must be > 0 */
extern int in(int v, set e);

extern void print(set e);

/* Build a set from an array "present[size]" of present variables */
extern set from_array(int size, int present[]);

/* Enumerate all sets included in 1..maxvar 
   and call successively "f" on all these sets.
*/
extern void enumall(int maxvar, void (*f)(set));

#endif
