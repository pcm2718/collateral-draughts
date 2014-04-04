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



/*
 * Utility function, not present in state.h.
 */
int
linearise_coordinates ( State * const state , short x , short y )
{
  return x + ( state->dim * y );
};



/*
 * Generally speaking, this is the piece of code with the most
 * complexity to it, it needs to be checked last *and* very throughly.
 *
 * I should optimize this function with caching.
 */
bool
state_move ( State * const state , short x , short y , short move )
{
  /*
   * Wrap the original coordinates.
   *
   * Might not want to do this.
   */
  wrap_coordinates ( state , &x , &y );
 

  /*
   * Cache the linear coordinates and the tile.
   *
   * Might do more caching in the future.
   */
  short linear = linearise_coordinates ( state , x , y );
  short tile = state->board[ linear ];


  /*
   * Test the tile to make sure it has a piece and the current player
   * is allowed to move the piece. If either of these conditions is
   * not met, cancel the move and return false.
   */
  if ( tile == TILE_EMPTY
       || ( state->player == 'r' && tile < 0 )
       || ( state->player == 'b' && tile > 0 ) )
    return false;


  /*
   * Set the capture flag to false and initialize the x and y
   * coordinates of the captured piece. By the end of move
   * computation,the capture flag indicates whether an enemy piece
   * has been captured, with capture_x and capture_y holding the
   * coordinates of the captured piece.
   *
   * The capture flag itself may be redundant.
   */
  bool capture = false;
  short capture_x = -1;
  short capture_y = -1;


  /*
   * Initialize the piece's post-move coordinates.
   */
  short new_x = x;
  short new_y = y;


  /*
    * Make sure the piece can be moved as indicated, essentially,
    * make sure pieces are not moving backwards if they are not
    * kings. If this is happening, cancel the move and return
    * false.
    */
  if ( /* Non-king test. */
       ( tile * tile < 4 )
       /* Direction test. */
       && ( tile * move < 0 ) )
    return false;


  /*
   * Move computation loop, the magic happens here.
   *
   * I might try and reorder the tests for improved efficiency.
   */
  do
    {
      /*
       * If the capture run has started, set the coordinates of the
       * possibly captured piece.
       */
      capture_x = new_x;
      capture_y = new_y;


      /*
       * Compute the piece's coordinates after the move. If the
       * capture flag is set, this piece of code will be run twice
       * to account for the jump over a piece.
       *
       * I might be able to change this to eliminate the comparison
       * on the capture run.
       */
      switch ( move )
        {
         case MOVE_SW:
          new_x -= 1;
          new_y -= 1;
          break;

        case MOVE_SE:
          new_x += 1;
          new_y -= 1;
          break;

        case MOVE_NE:
          new_x += 1;
          new_y += 1;
          break;

        case MOVE_NW:
          new_x -= 1;
          new_y += 1;
          break;

        case MOVE_NO:
        default:
          /*
           * Do nothing.
           */
          break;
        };


      /*
       * Make sure the post-move coordinates are on the board, that
       * the piece has not landed on a friendly piece and, if the
       * piece has already jumped an enemy piece, that the piece has
       * not landed on an enemy piece. If any of these conditions are
       * true, cancel the move and return false.
       *
       * Could I improve this test?
       */
      if ( ( ! valid_coordinates ( state , new_x , new_y ) )
           || ( tile * state->board[ linearise_coordinates ( state , new_x , new_y ) ] > 0 )
           || ( capture && ( tile * state->board[ linearise_coordinates ( state , new_x , new_y ) ] < 0 ) ) )
        return false;


      /*
       * If the capture flag is set the loop has run twice, so we are
       * finished computing the move. Break from the move loop.
       */
      if ( capture )
        break;


      /*
       * At this point, we know that the piece has landed on either
       * an empty space or an enemy piece. Set the capture flag
       * accordingly.
       */
      capture = ( tile * state->board[ linearise_coordinates ( state , new_x , new_y ) ] );
    }
  while ( capture );


  /*
   * If we get this far, the user's move is legitimate and the capture
   * flag and capture coordinates have been set in the event of a
   * capture.
   */


  /*
   * Make the actual move. DO NOT substitute the cached values here,
   * the actual array needs to change here.
   */
  short tmp = tile;
  state->board[ linear ] = state->board[ linearise_coordinates ( state , new_x , new_y ) ];
  state->board[ linearise_coordinates ( state , new_x , new_y ) ] = tmp;


  /*
   * If the capture flag has been set, remove the captured piece.
   */
  if ( capture )
    state->board[ linearise_coordinates ( state , capture_x , capture_y ) ] = TILE_EMPTY;


  /*
   * If we got this far, the move was successful, so return true.
   */
  return true;
};



bool
state_load ( State * const state , FILE * loadfile )
{
  /*
   * Note: this is an extremely limited parser, which will need to be
   * modified to support the full functionality of the system it lies
   * beneath.
   *
   * Add error checking?
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
   * Free the old board.
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

          state->board[ linearise_coordinates ( state , i , j ) ] = tile;
        }
    }

  /*
   * Return success.
   */
  return true;
};



/*
 * Noticed a problem, should the state argument be made const, or
 * should this function return void?
 */
bool
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
  return true;
};



/*
 * Might need to add error handling to this.
 */
bool
state_tget ( State * const state , short x , short y , short* getval )
{
  /*
   * "Wrap" x and y around the board, if necessary.
   *
   * I don't really know whether out-of-bounds x and y values should
   * be treated as errors or wrapped around, time will tell.
   */
  wrap_coordinates ( state , &x , &y );

  /*
   * Set getval.
   */
  *getval = state->board[ x + ( state->dim * y ) ];

  /*
   * Return success.
   */
  return true;
};



/*
 * Might need to add error-handling to this.
 */
bool
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

  /*
   * Return success.
   */
  return true;
};



bool
state_has_moves ( State const * const state , char const * const player )
{
  return true;
};
