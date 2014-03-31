#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "state.h"
#include "cmd.h"



#define INIT_FILENAME "../../home/examples/checkers_init.dat"
#define PLAYER_COUNT 2
//#define CMD_BUFFER_SIZE 128



int
main ( int argc , char** argv )
{
  /*
   * Do initialization steps here.
   */

  /*
   * Set up list of player id's.
   */
  char* players [2] = { "r" , "b" };

  /*
   * Attempt to initialize the game state to the default.
   *
   * Add error checking?
   */
  State* state = build_state ( );
  FILE* init_file = fopen ( INIT_FILENAME , "r" );
  state = state_load ( state , init_file );
  fclose ( init_file );


  /*
   * The top level of program logic resides nested within this loop,
   * which runs until one of the players sends the termination signal.
   *
   * Note that this is an infinite loop. The actual control logic for
   * this loop is a single goto statment in the verb interpreter, set
   * to jump outside the loop if and only if the VERB_TERM is
   * encountered. DO NOT BE ALARMED. THIS IS A LEGITIMATE USE OF THE
   * GOTO STATEMENT!
   */
  while ( true )
    {
      /*
       * The logic inside this loop effectively handles a single
       * round, from the starting position to reseting the game.
       */


      /*
       * Initialize the player indexer and player.
       */
      unsigned short indexer = 0;
      char* player = players[0];


      /*
       * While a game is still in progress (i.e. while both players
       * have legal moves available), this loop alternates between
       * the red and black players, starting with the red player.
       *
       * The control statement for this loop determines whether the
       * player has any vaild moves. If the player does not have any
       * valid moves, the control logic of the interpreter loop is
       * modified to force the losing player to fix the situation.
       * This could change in the future, so with minor changes,
       * control could just be made to fall through to a reset
       * after this loop, it just depends on what happens during
       * development.
       */
      while ( state_has_moves ( state , player ) )
        {
          /*
           * The logic inside this loop effectively handles a single
           * turn, allowing an unlimited number of commands which are
           * not moves to be performed, followed by a single move.
           *
           * Notice that both players must be trusted for this system
           * to work as-is, otherwise any player could modify the
           * state of the game to their advantage. This is a standing
           * security issue, but one which can be ignored at the
           * moment for the academic purposes of the project.
           */

          /*
           * Initialize the keep_turn flag.
           */
          bool keep_turn = true;

          /*
           * While the player has not yet made a move, it is still
           * the player's turn. Thus, this loop terminates only after
           * a move has been made, processing any other commands
           * the player gives during the current turn. Control stays
           * with the player anyways if he has no moves, as he has
           * lost and the losing player is responsible for resetting
           * the board.
           */
          while ( keep_turn || ( ! state_has_moves ( state , player ) ) )
            {
              /*
               * This logic effectively handles a single command from
               * the current player. This involves reading the command
               * from the player's input pipe, interpreting and
               * executing the command, and returning the result of
               * the command to the player as appropriate.
               */

              /*
               * Initialize the command buffer.
               */
              //char* cmd = malloc ( sizeof ( char ) * CMD_BUFFER_SIZE );
              char* cmd = NULL;
              int cmdlen = 0;

              /*
               * Read the next command from stdin.
               *
               * Might try and modify this to avoid the buffer.
               */
              getline ( cmd , cmdlen , stdin );

              /*
               * Extract the verb of the command, this is always the
               * first word of the command, in a HTTP-like format.
               */
              char* verb = strtok ( cmd , " \n" );


              /*
               * Use an if-else chain statement to select the proper
               * code to execute for each verb.
               */
              if ( ! verb )
                {
                  /*
                   * If the entry was just a newline, ignore it.
                   */
                }
                
              else if ( ! strcmp ( verb , VERB_TERM ) )
                {
                  /*
                   * If the term signal is given, then jump to the
                   * term_exit branch.
                   */

                  goto term_exit;
                }

              else if ( ! strcmp ( verb , VERB_LOAD ) )
                {
                  cmd_load ( cmd , state );
                }

              else if ( ! strcmp ( verb , VERB_SAVE ) )
                {
                  cmd_save ( cmd , state );
                }

              else if ( ! strcmp ( verb , VERB_TGET ) )
                {
                  cmd_tget ( cmd , state );
                }

              else if ( ! strcmp ( verb , VERB_TSET ) )
                {
                  cmd_tset ( cmd , state );
                }

              else if ( ! strcmp ( verb , VERB_MOVE ) )
                {
                  /*
                   * This code executes if the verb is the move command.
                   * It then parses the remainder of the command to
                   * determine its validity. If the command is well-
                   * structured and the move is legal, the move is made
                   * and the moved flag set to true. Otherwise, a 400
                   * BAD SYNTAX error will be returned. If the move
                   * requested is well-formed but illegal, a 403 MOVE
                   * FORBIDDEN error is returned. Assuming no errors
                   * occur, a 200 OK message is returned.
                   */

                  cmd_move ( cmd , state );

                  keep_turn = false;
                }

              else
                {
                  /*
                   * If we don't recognize the given verb, we do the
                   * safe thing and return the verb nonexistant error.
                   */
                  //puts ( "PCIP/1.0 404 NOTVERB" );
                  puts ( "NULL" );
                }


              /*
               * Deallocate the command buffer.
               */
              free ( cmd );
           }
          
          /*
           * Increment the player indexer with wraparound. Set the
           * pipes pointing to the current player's input and
           * output.
           *
           * Trust me, I have a reason for putting it here.
           */
          indexer = ( indexer + 1 ) % PLAYER_COUNT;
          player = players[indexer];

          /*
           * Announce the turn change.
           */
          printf ( "PCIP/1.0 201 NEW TURN\n%c\n" , *player );
       }


      /*
       * This is the interpreter loop's aforementioned implicit fall
       * through case for processing end of game conditions.
       *
       * Given the termination condition of the previous loop, the
       * player input and output pipes now point to the loosing
       * player, who is now responsible for resetting the game.
       */
    }


  /*
   * If the term command is given, the program jumps to this point,
   * just before deallocation of top-level dynamic resources occurs.
   * Note that breaking out of a switch statement and a triple nested 
   * do-while loop *is* a legitimate use of goto.
   */
term_exit:


  /*
   * Clean up.
   */

  /*
   * Deallocate the game state.
   */
  free_state ( state );


  /*
   * If we got this far without an incident, the program should have
   * run successfully, so we return the corresponding status code.
   */
  return EXIT_SUCCESS;
};
