import java.io.*;
import java.util.*;
import java.net.*;
import java.awt.*;
import java.awt.event.*;
import javax.swing.*;



public class GameBoard extends JPanel
{
        /*
         * The array of buttons representing squares on the board.
         */
        ArrayList<CheckerButton> game_board;



        /*
         * The grid layout defines the layout of the board's squares.
         */
        GridLayout board_layout;



        /*
         * The server end of the sockets connection.
         */
        ServerSocket server_socket;



        /*
         * The client end of the sockets connection.
         */
        Socket client_socket;



        /*
         * Constructor.
         *
         * Need to set up some try statements to catch initialization
         * errors and decide which tests might fail first.
         */
        GameBoard ( int port_number ) throws IOException
        {
                /*
                 * Initialize the pipes for communicating with Meyrl.
                 */

                /*
                 * Initialize the server socket on port port_number.
                 */
                server_socket = new ServerSocket ( port_number );

                /*
                 * Initialize the array of squares.
                 */
                game_board = new ArrayList<CheckerButton> ();
                game_board.add ( new CheckerButton ( 1 ) );

                /*
                 * Initialize the layout for the board.
                 */
                this.setLayout ( new GridLayout (0, 2) );

                /*
                 * Put the squares onto the panel.
                 */
                for ( int i = 0 ; i < 1 ; ++i )
                {
                        this.add ( game_board.get ( i ) );
                }
        }



        /*
         * Main game loop.
         */
        int main_loop ()
        {
                /*
                 * This is the primary game loop on the human's side
                 * of the game.
                 */
                /*
                while ( false )
                {
                }
                */

                /*
                 * Return the status of the game after the main loop
                 * terminates.
                 */
                return 0;
        }



        /*
         * Get a sockets connection from the client.
         */
        /*
        Socket await_connection ()
        {
        }
        */



        /*
         * Send text to an instance of Meyrl via piping.
         */
        // NYI



        /*
         * Get text from an instance of Meyrl via piping.
         */
        // NYI
}
