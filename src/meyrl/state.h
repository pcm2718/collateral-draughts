#ifndef MEYRL_STATE_H
#define MEYRL_STATE_H



#define _GNU_SOURCE



#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>



/*
 * Tile codes.
 */
#define TILE_B_KING -2
#define TILE_B_MAN -1
#define TILE_EMPTY 0
#define TILE_R_MAN 1
#define TILE_R_KING 2

/*
 * Tile chars.
 */
#define CHAR_B_KING 'B'
#define CHAR_B_MAN 'b'
#define CHAR_EMPTY '.'
#define CHAR_R_MAN 'r'
#define CHAR_R_KING 'R'

/*
 * Move codes.
 *
 * Can I make these more independent of the state?
 */
#define MOVE_SW -2
#define MOVE_SE -1
#define MOVE_NO 0
#define MOVE_NE 1
#define MOVE_NW 2

/*
 * Move strs.
 *
 * Can I make these more independent of the state?
 */
#define STR_SW "SW\0"
#define STR_SE "SE\0"
#define STR_NO "NO\0"
#define STR_NE "NE\0"
#define STR_NW "NW\0"



/*
 * This data structure contains the state of the game, everthing
 * needed to represent a game of Checkers is right here.
 *
 * Alternative Title:
 *
 * Everything You Always Wanted to Know About a Game of Checkers *
 * * But Were Afraid to Ask
 */
struct state 
{
  /*
   * The width and length of the board.
   */
  unsigned short dim;

  /*
   * The array containing the board tiles.
   */
  short* board;

  /*
   * The current player.
   */
  char player;
};
typedef struct state State;



State*
build_state ( );



void
free_state ( State* state );



State*
state_load ( State * const state , FILE* loadfile );



State*
state_save ( State * const state , FILE* savefile );



short
state_tget ( State * const state , short x , short y );



void
state_tset ( State * const state , short x , short y , short setval );



bool
state_move ( State * const state , short x , short y , short move );



bool
state_has_moves ( State const * const state , char const * const player );



bool
state_set_player ( State * const state , char const * const player );



#endif
