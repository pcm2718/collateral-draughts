#ifndef MEYRL_CMD_H
#define MEYRL_CMD_H



#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include "state.h"



/*
 * Verb definitions.
 */

#define VERB_MOVE "move\0"
#define VERB_LOAD "load\0"
#define VERB_SAVE "save\0"
#define VERB_TERM "term\0"
#define VERB_TGET "tget\0"
#define VERB_TSET "tset\0"



State*
cmd_move ( char * const args , State * const state );



State*
cmd_load ( char * const args , State * const state );



State*
cmd_save ( char * const args , State * const state );



State*
cmd_tget ( char * const args , State * const state );



State*
cmd_tset ( char * const args , State * const state );



#endif
