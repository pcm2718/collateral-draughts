#ifndef MEYRL_CMD_H
#define MEYRL_CMD_H



#define _GNU_SOURCE



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



bool
cmd_move ( char * const cmd , State * const state );



bool
cmd_load ( char * const cmd , State * const state );



bool
cmd_save ( char * const cmd , State * const state );



bool
cmd_tget ( char * const cmd , State * const state );



bool
cmd_tset ( char * const cmd , State * const state );



#endif
