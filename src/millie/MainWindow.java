import java.io.*;
import java.util.*;
import java.net.*;
import java.awt.*;
import java.awt.event.*;
import javax.swing.*;

import javax.swing.JMenu;
import javax.swing.JMenuItem;
import javax.swing.JMenuBar;

public class MainWindow extends JFrame
{
        public static void main(String args[]) throws IOException
        {
                new MainWindow ();
        }



        MainWindow() throws IOException
        {
                /*
                 * Set window name.
                 */
                super ( "Millie" );

                /*
                 * Set window bounds.
                 */
                setBounds ( 160 , 120 , 640 , 480 );

                /*
                 * Make sure the program quits when the window is closed.
                 */
                setDefaultCloseOperation ( JFrame.EXIT_ON_CLOSE );

                /*
                 * Create a menu bar and add it to the frame.
                 */
                JMenuBar menu_bar = new JMenuBar ();

                menu_bar.add ( new JMenu ( "File" ) );

                this.setJMenuBar ( menu_bar );
                //menu_bar.setOpaque ( true );

                /*
                 * Get the container.
                 */
                Container container = this.getContentPane ();

                /*
                 * Add an instance of GameBoard to the center of the
                 * content pane.
                 */
                this.getContentPane ().add ( new GameBoard ( 1969 ) , BorderLayout.CENTER );

                /*
                 * Add a menu bar to the window.
                 */


                /*
                 * Make sure the window is visible.
                 */
                this.setVisible (true);
        }
}
