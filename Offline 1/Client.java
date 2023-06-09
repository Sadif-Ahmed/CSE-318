import java.util.Arrays;
import java.util.Scanner;
import java.io.*;


 public class Client{
      public static void main(String[] args){
          Scanner in = null;
          
          try {
            //File file = new File("input.txt");
            in = new Scanner(System.in);
              
          } catch (Exception e) {
              //TODO: handle exception
          }
          
          System.out.println("Enter Grid Size:");
          
          
          int k = in.nextInt();
          int [][]mat  = new int[k+1][k+1];
          System.out.println("Enter Intial Board State");
          
          for (int i = 1; i <= k; i++) {
            for (int j = 1; j <= k; j++) {
                String c = in.next();
                try {
                    mat[i][j]=Integer.parseInt(c);  
                    
                } catch (Exception e) {
                    //TODO: handle exception
                }
                //mat[i][j]=in.nextInt();
            }
          }
          System.out.println("Initial Board State: ");
          for (int i = 1; i <= k; i++) {
            for (int j = 1; j <= k; j++) {
                System.out.print(mat[i][j]);
            }
            System.out.println("");
          }
          int []moves_store = {0,0};
          Puzzle_Solver ps = new Puzzle_Solver(mat , k);
          Search_Node goal;
          for(int i=1 ; i<=2 ; i++){
              ps.set_heuristic_choice(i);
              if(ps.check_solvability())
              {
              goal = ps.solve();
              moves_store[i-1]=ps.move_count(goal);
              }
          }
        if(ps.check_solvability())
        {
        if(moves_store[0]<=moves_store[1])
        {
            System.out.println("Minimum Number of moves = "+moves_store[0]);
            ps.set_heuristic_choice(1);
            goal = ps.solve();
            ps.print_path(goal);
        }
        else
        {
            System.out.println("Minimum Number of moves = "+moves_store[1]);
            ps.set_heuristic_choice(2);
            goal = ps.solve();
            ps.print_path(goal);

        }
        }
        else
        {
            System.out.println("Unsolvable Puzzle");
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
