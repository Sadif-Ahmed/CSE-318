import java.util.HashSet;
import java.util.Hashtable;
import java.util.*;
class Pair{
    int x;
    int y;
    Pair(int x,int y)
    {
        this.x=x;
        this.y=y;
    }
    int get_x()
    {
        return x;
    }
    int get_y()
    {
        return y;
    }
}
public class Puzzle_Solver {
    Board startBoard;
    Board goalBoard;
    //int expanded_count;
   // int explored_count;
    int size;
    Hashtable<Board, Integer> open_list;
    Hashtable<Board, Integer> close_list;
    HashSet<Board> visited;
    PriorityQueue<Search_Node> queue;
    Heuristics heuristics;
    final int max_list_size=10000000;

    public Puzzle_Solver(int [][] mat, int size)
    {
        open_list =  new Hashtable<>(max_list_size);
        close_list = new Hashtable<>(max_list_size);
        queue =  new PriorityQueue<>(max_list_size, new Node_Comp());
        heuristics =  new Heuristics();
        this.size=size;
        startBoard = new Board(mat, size);
        goalBoard = Board.get_goal(size);
        visited = new HashSet<Board>();
    }
    Pair get_blank_pos(Board board)
    {
        for(int i=1;i<=board.size;i++)
        {
            for(int j=1;j<=board.size;j++)
            {
                if(board.mat[i][j]==0)
                {
                    return new Pair(i, j);
                }
            }
        }
        return null;
    }
    public void clear_list()
    {
        queue.clear();
        open_list.clear();
        close_list.clear();
        visited.clear();
    }
    public Search_Node solve(int h)
    {   
        
        int staring_h_n;
        if(h==1)
        {
            staring_h_n = heuristics.hamming_distance(startBoard); 
        }
        else
        {
            staring_h_n = heuristics.manhattan_distance(startBoard);
        }
        Search_Node node = new Search_Node(startBoard, 0, staring_h_n);
        node.parent=null;
        clear_list();
        queue.add(node);
        visited.add(startBoard);
       // expanded_count=0;
       // explored_count=0;
        
        int []change_x={1,-1,0,0};
        int []change_y={0,0,1,-1};

        while(!queue.isEmpty())
        {
           // expanded_count++;     
            Search_Node current =  queue.remove();
            Pair p =  get_blank_pos(current.board);
            int x,y;
            for(int i=0;i<4;i++)
            {
                x=p.get_x()+change_x[i];
                y=p.get_y()+change_y[i];

                if(x<1 || x> size || y<1 || y>size) continue;
                
                Board child = current.board.swap_cell(p.get_x(), p.get_y(), x, y);

                if(visited.contains(child)) continue;
                int child_h_n;
                if(h==1)
                {
                   child_h_n = heuristics.hamming_distance(child); 
                }
                else
                {
                    child_h_n = heuristics.manhattan_distance(child);
                }
                Search_Node child_node = new Search_Node(child, current.g_n+1, child_h_n);
                child_node.parent=current;
                queue.add(child_node);
                //explored_count++;
                if(child.equals(goalBoard))
                {
                    return child_node;
                }
            }
            visited.add(current.board);
        }
        System.out.println("No Solution Found!");
        return null;
    }
    public int move_count(Search_Node goal)
    {
        int count=0;
        while(goal!=null)
        {
            goal = goal.parent;
            count++;
        }
        return count-1;
    }
    public void print_path(Search_Node goal)
    {
        Stack <Search_Node> path = new Stack<>();
        while(goal!=null)
        {
            path.push(goal);
            goal=goal.parent;
        }
        while(!path.isEmpty())
        {
            Search_Node temp = path.pop();
            temp.board.print();
            System.out.println("---");
            
        }
    }
}
