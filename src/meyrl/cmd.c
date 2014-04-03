#include "cmd.h"



bool
cmd_move ( char * const cmd , State * const state )
{
  /*
   * Initialize the x and y coordinates and the move code.
   */
  short x = 0;
  short y = 0;
  short move = MOVE_NO;

  /*
   * Get the x and y coordinates  and the move from the command.
   */
  char* x_str = strtok ( NULL , " \n" );
  char* y_str = strtok ( NULL , " \n" );
  char* move_str = strtok ( NULL , " \n" );

  /*
   * If any of the argument strings are NULL, we don't have enough
   * information to make a move. Return failure.
   */
  if ( ! ( x_str && y_str && move_str ) )
    return false;

  /*
   * Convert the args to shorts.
   */
  sscanf ( x_str , "%hi" , &x );
  sscanf ( y_str , "%hi" , &y );
  sscanf ( move_str , "%hi" , &move );

  /*
   * Return the status of the move.
   */
  return state_move ( state , x , y , move );

  /*
   * If we made it this far, the move has been made, so return true.
   */
  //return true;
};



bool
cmd_load ( char * const cmd , State * const state )
{
  /*
   * Try and pull a filename off the command.
   */
  char* filename = strtok ( NULL , " \n" );

  /*
   * Initialize the return value to false, for failure.
   */
  bool retval = false;
  
  if ( filename )
    {
      /*
       * If a filename was given, try and save to filename.
       */

      /*
       * Attempt to open the file. If successful, load the state from
       * the file. Close the file.
       */
      FILE* loadfile = fopen ( filename , "r" );
      retval = loadfile && state_load ( state , loadfile );
      fclose ( loadfile );
    }
  else
    {
      /*
       * If no filename was provided, load from stdin.
       */

      retval = state_load ( state , stdin );
    }

  /*
   * Return the retval.
   */
  return retval;
};



bool
cmd_save ( char * const cmd , State * const state )
{
  /*
   * Try and pull a filename off the command.
   */
  char* filename = strtok ( NULL , " \n" );

  /*
   * Initialize the return value to false, for failure.
   */
  bool retval = false;

  if ( filename )
    {
      /*
       * If a filename was given, try and save to filename.
       */

      /*
       * Attempt to open the file. If successful, save the state to
       * the file. Close the file.
       */
      FILE* savefile = fopen ( filename , "w" );
      retval = savefile && state_save ( state , savefile );
      fclose ( savefile );
   }
  else
    {
      /*
       * If no filename was provided, save to stdout.
       */

      retval = state_save ( state , stdout );
    }

  /*
   * Return the retval.
   */
  return retval;
};



bool
cmd_tget ( char * const cmd , State * const state )
{
  return false;
};



bool
cmd_tset ( char * const cmd , State * const state )
{
  return false;
};
