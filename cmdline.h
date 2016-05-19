/* Treatment of command-line for prefix and infix parsers.
*/

#ifndef _CMDLINE_H
#define _CMDLINE_H 1

#include "sorts.h"
typedef Prop (*parser)();

void run(parser proc, const char *parser_name, int argc, char *argv[]);


#endif
