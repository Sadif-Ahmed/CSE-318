
import java.util.Scanner;

 public class Client{
      public static void main(String[] args){
          Scanner in = null;
          
          try {
            
            in = new Scanner(System.in);
              
          } catch (Exception e) {
              //TODO: handle exception
          }
          
          System.out.println("Enter Grid Size:");
          
          
          String x = in.next();
          int k=Integer.parseInt(x);
          int [][]mat  = new int[k+1][k+1];
          System.out.println("Enter Intial Board State:");
          
          for (int i = 1; i <= k; i++) {
            for (int j = 1; j <= k; j++) {
                String c = in.next();
                try {
                    mat[i][j]=Integer.parseInt(c);  
                    
                } catch (Exception e) {
                    //TODO: handle exception
                }
               
            }
          }
          System.out.println("Initial Board State: ");
          for (int i = 1; i <= k; i++) {
            for (int j = 1; j <= k; j++) {
                System.out.print(mat[i][j]+" ");
            }
            System.out.println("");
          }
          System.out.println("");
          if(k<=3)
          {
          int []moves_store = {0,0};
          Puzzle_Solver ps = new Puzzle_Solver(mat , k);
          Search_Node goal;
          for(int i=1 ; i<=2 ; i++){
              if(Board.check_solvability(new Board(mat, k)))
              {
              goal = ps.solve(i);
              moves_store[i-1]=ps.move_count(goal);
              }
          }
        if(Board.check_solvability(new Board(mat, k)))
        {
        if(moves_store[0]<=moves_store[1])
        {
            System.out.println("Minimum Number of moves = "+moves_store[0]);
            goal = ps.solve(1);
            System.out.println("");
            ps.print_path(goal);
        }
        else
        {
            System.out.println("Minimum Number of moves = "+moves_store[1]);
            goal = ps.solve(2);
            System.out.println("");
            ps.print_path(goal);

        }
        }
        else
        {
            System.out.println("Unsolvable Puzzle");
        }
          
    }
    else
    {
        if(Board.check_solvability(new Board(mat, k)))
        {
      Puzzle_Solver puzzle = new Puzzle_Solver(mat, k);
      Search_Node t = puzzle.solve(2);
      System.out.println("Minimum Number of moves = " + puzzle.move_count(t) );
      System.out.println("");
      puzzle.print_path(t);
      }
      else
      {
        System.out.println("Unsolvable Puzzle");
      }
    }
    }  
   
 }
