import java.awt.Color;
import java.awt.Graphics;
import javax.swing.JButton;



public class CheckerButton extends JButton
{
        int state;



        CheckerButton ( int inital_state )
        {
                state = inital_state;
        }



        @Override protected void paintComponent ( Graphics g )
        {
                super.paintComponent ( g );

                /*
                 * Might be able to replace this switch statement
                 * with behavior based on odd numbers and value.
                 *
                 * May also be able to merge some case statements.
                 */
                switch ( state )
                {
                        /*
                         * Draw a red checker.
                         */
                        case 1:
                                g.setColor ( Color.RED );
                                g.fillOval ( 10 , 10 , 10 , 10 );
                                break;

                        /*
                         * Draw a red king.
                         */
                        case 2:
                                break;

                        /*
                         * Draw a black checker.
                         */
                        case 3:
                                break;

                        /*
                         * Draw a black king.
                         */
                        case 4:
                                break;

                        /*
                         * No reason the blank square and default
                         * cases shouldn't share a behavior.
                         */
                        case 0:
                        default:
                                break;
                }
        }
}
