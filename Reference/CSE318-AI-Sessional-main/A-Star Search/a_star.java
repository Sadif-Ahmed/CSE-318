import java.util.Arrays;
import java.util.Scanner;
import java.io.*;

/**
 *
 * @author Afnan
 */

 public class a_star{
      public static void main(String[] args){
          Scanner in = null;
          
          try {
            //File file = new File("input.txt");
            in = new Scanner(System.in);
              
          } catch (Exception e) {
              //TODO: handle exception
          }
          
          System.out.println("Enter Grid Size");
          
          
          int k = in.nextInt();
          int [][]mat  = new int[k+1][k+1];
          //System.out.println("Enter Intial Board State");
          
          for (int i = 1; i <= k; i++) {
            for (int j = 1; j <= k; j++) {
                String c = in.next();
                try {
                    if(c.equalsIgnoreCase("*"))mat[i][j]=0;
                    else mat[i][j]=Integer.parseInt(c);  
                    
                } catch (Exception e) {
                    //TODO: handle exception
                }
                //mat[i][j]=in.nextInt();
            }
          }
          for (int i = 1; i <= k; i++) {
            for (int j = 1; j <= k; j++) {
                System.out.print(mat[i][j]);
            }
            System.out.println("");
          }

          for(int i=3 ; i>=1 ; i--){
              if(i==1){
                  System.out.println("Heuristics : Hamming distance");
              }
              else if(i==2){
                  System.out.println("Heuristics : Manhattan Distance");
              }
              else{
                  System.out.println("Heuristics : Linear Conflicts");
              }

              board_solver bs = new board_solver(mat , k);
              bs.set_heuristic_id(i);
              Node goal = bs.solve();
              if(goal != null){
                  System.out.println("Cost : " + goal.f_cost);
                  bs.print_path(goal);
              }
          }
          
      
      

      //for lightoz test cases
       /*
      int n = in.nextInt();
      int [][]mat  = new int[4][4];
      int tc=0;
      while(n>0){
        for (int i = 1; i <= 3; i++) {
            for (int j = 1; j <= 3; j++) {
                mat[i][j] = in.nextInt();
            }
        }
        String s = in.nextLine();

        board_solver bs = new board_solver(mat , 3);
        bs.set_heuristic_id(3);
        Node goal = bs.solve();
        if(goal != null){
            System.out.println("Case "+ (++tc) + ": " + goal.f_cost);
            //bs.print_path(goal);
        }
        else{
            System.out.println("Case "+ (++tc) + ": impossible");
        }
        n--;  
      }
      */
    }  
 }