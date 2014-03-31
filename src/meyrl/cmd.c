#include "cmd.h"



bool
cmd_move ( char * const cmd , State * const state )
{
  return false;
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
