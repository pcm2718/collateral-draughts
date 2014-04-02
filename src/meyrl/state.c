#include "state.h"



State*
build_state ( )
{
  State* state = malloc ( sizeof ( State ) );

  state->dim = 0;
  state->board = NULL;
  state->player = '\0';

  return state;
};



void
free_state ( State* state )
{
  free ( state->board );

  free ( state );
};



/*
 * Utility function, not present in state.h.
 */
bool
valid_coordinates ( State * const state , short const x , short const y )
{
  return ( x / state->dim || y / state->dim || x < 0 || y < 0 ) ? false : true ;
};



/*
 * Utility function, not present in state.h.
 */
void
wrap_coordinates ( State * const state , short* x , short* y )
{
  *x = *x % state->dim;
  *y = *y % state->dim;
};



bool
state_move ( State * const state , short x , short y , short direction )
{
  /*
   * Might want to do a player check here.
   */

  /*
   * Wrap the original coordinates.
   *
   * Might not want to do this.
   */
  wrap_coordinates ( state , &x , &y );

  /*
   * Initialize the post-move coordinates.
   */
  short new_x = x;
  short new_y = y;

  /*
   * Compute the new x-y coordinates post move.
   *
   * Might be able to modify enum values to share move code.
   */
  switch ( direction )
    {
    case MOVE_NE:
      new_x += 2;
      new_y += 2 * state->dim;
      break;

    case MOVE_SE:
      new_x += 2;
      new_y -= 2 * state->dim;
      break;

    case MOVE_SW:
      new_x += 2;
      new_y -= 2 * state->dim;
      break;

    case MOVE_NW:
      new_x -= 2;
      new_y -= 2 * state->dim;
      break;
    };

  /*
   * Validate the new coordinates. If coordinates are not valid,
   * return a false immediately, otherwise, execute the move.
   */
  if ( ! valid_coordinates ( state , x , y ) )
    return false;

  short tmp = state->board[ x + ( state->dim * y ) ];
  state->board[ x + ( state->dim * y ) ] = state->board[ new_x + ( state->dim * new_y ) ];
  state->board[ new_x + ( state->dim * new_y ) ] = tmp;

  /*
   * If we made it this far, the move was successful, so return true;
   */
  return true;
};



State*
state_load ( State * const state , FILE * loadfile )
{
  /*
   * Note: this is an extremely limited parser, which will need to be
   * modified to support the full functionality of the system it lies
   * beneath.
   */

  /*
   * Read the size of the board.
   */
  fscanf ( loadfile , "%hu %hu\n" , &state->dim , &state->dim );

  /*
   * Read the current player.
   */
  fscanf ( loadfile , "%c\n" , &state->player );

  /*
   * Free the board array.
   *
   * Might be able to merge this with the step below.
   */
  free ( state->board );

  /*
   * Allocate the board array.
   */
  state->board = malloc ( sizeof ( short ) * state->dim * state->dim );

  /*
   * Read the board.
   *
   * To be frank, this part of the parser is ugly and fragile, might
   * be able to fix it with clever use of getdelim.
   */
  for ( short j = state->dim - 1 ; j >= 0 ; --j )
    {
      for ( short i = 0 ; i < state->dim ; ++i )
        {
          char tile_char = CHAR_EMPTY;
          fscanf ( loadfile , "%c " , &tile_char );

          short tile = TILE_EMPTY;
          switch ( tile_char )
            {
            case CHAR_R_MAN:
              tile = TILE_R_MAN;
              break;

            case CHAR_R_KING:
              tile = TILE_R_KING;
              break;

            case CHAR_B_MAN:
              tile = TILE_B_MAN;
              break;

            case CHAR_B_KING:
              tile = TILE_B_KING;
              break;

            case CHAR_EMPTY:
              tile = TILE_EMPTY;
              break;
            }

          state->board[ i + ( state->dim * j ) ] = tile;
        }
    }

  /*
   * Return the state.
   */
  return state;
};



/*
 * Noticed a problem, should the state argument be made const, or
 * should this function return void?
 */
State*
state_save ( State * const state , FILE * savefile )
{
  /*
   * Note: this is an extremely limited generator, which will need to
   * bemodified to support the full functionality of the system it
   * lies beneath.
   */

  /*
   * Write the size of the board.
   */
  fprintf ( savefile , "%hu %hu\n" , state->dim , state->dim );

  /*
   * Write the current player.
   */
  fprintf ( savefile , "%c\n" , state->player );

  /*
   * Write the board.
   *
   * Might be able to improve speed using terenary operator and mod.
   */
  for ( short j = state->dim - 1 ; j >= 0 ; --j )
    {
      for ( short i = 0 ; i < state->dim ; ++i )
        {
          char tile_char = CHAR_EMPTY;

          switch ( state->board[i + ( state->dim * j )] )
            {
            case TILE_R_MAN:
              tile_char = CHAR_R_MAN;
              break;

            case TILE_R_KING:
              tile_char = CHAR_R_KING;
              break;

            case TILE_B_MAN:
              tile_char = CHAR_B_MAN;
              break;

            case TILE_B_KING:
              tile_char = CHAR_B_KING;
              break;

            case TILE_EMPTY:
              tile_char = CHAR_EMPTY;
              break;

            default:
              tile_char = '?';
            }

          fprintf ( savefile , "%c " , tile_char );
        }
      fputc ( '\n' , savefile );
    }

  /*
   * Return the state.
   */
  return state;
};



/*
 * Might need to add error handling to this.
 */
short
state_tget ( State * const state , short x , short y )
{
  /*
   * "Wrap" x and y around the board, if necessary.
   *
   * I don't really know whether out-of-bounds x and y values should
   * be treated as errors or wrapped around, time will tell.
   */
  wrap_coordinates ( state , &x , &y );

  /*
   * Return the value of the indicated tile.
   */
  return state->board[ x + ( state->dim * y ) ];
};



/*
 * Might need to add error-handling to this.
 */
void
state_tset ( State * const state , short x , short y , short setval )
{
  /*
   * "Wrap" x and y around the board, if necessary.
   *
   * I don't really know whether out-of-bounds x and y values should
   * be treated as errors or wrapped around, time will tell.
   */
  wrap_coordinates ( state , &x , &y );

  /*
   * Set the value of the indicated tile.
   */
  state->board[ x + ( state-> dim * y ) ] = setval;
};



bool
state_has_moves ( State const * const state , char const * const player )
{
  return true;
};
