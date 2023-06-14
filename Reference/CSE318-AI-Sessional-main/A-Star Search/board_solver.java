import java.util.Arrays;
import java.util.*;
/**
 *
 * @author Afnan
 */

 public class board_solver{

    Board initial_board;
    Board final_board;
    int node_expanded;
    int node_explored;
    int board_size;
    Hashtable<Board, Integer> open_list;
    Hashtable<Board, Integer> close_list;
    HashSet<Board>visited;
    PriorityQueue<Node> pq;
    int h_id;
    heuristics h;
    final int list_size = 100000;

    public board_solver(int [][] matrix,int n){
        open_list = new Hashtable<>(list_size);
        close_list = new Hashtable<>(list_size);
        pq = new PriorityQueue<>(list_size , new NodeComparator());
        h = new heuristics();
        board_size = n;
        initial_board = new Board(matrix , n);
        final_board = Board.get_final_state(n);
        visited = new HashSet<Board>();
    }

    
    public Pair get_blank_position(Board b){
        Pair p = null;
        for(int i=1;i<=b.size;i++){
            for(int j=1;j<=b.size;j++){
                if(b.matrix[i][j]==0){
                    return new Pair(i,j);
                }
            }
        }
        return p;
    }

    public void set_heuristic_id(int i){
        h_id = i;
    }

    
    public int heuristic_value(Board b){
        if(h_id==1){
            return h.hamming_distance(b);
        }
        else if(h_id==2){
            return h.manhattan_distance(b);
        }
        else{
            return h.linear_conflicts(b);
        }
    }

    public void clear_list_q(){
        pq.clear();
        open_list.clear();
        close_list.clear();
        visited.clear();
    }

    public Node solve() {
        //first chk solvability
        int inv = initial_board.get_inversion_count();
        if(board_size %2 == 1){
            System.out.println("Board Size Odd!\tInversions: "+ inv);
            
            if((inv) % 2 == 0){
                System.out.println("Solution Exists!");
            }
            else{
                System.out.println("Solution doesnt Exist!!");
                return null;
            }
        }
        else{
            int blank_row = get_blank_position(initial_board).get_x();
            System.out.println("Board Size even!\tInversions: "+ inv + "\t Blank is in row " + blank_row);
        
            if((blank_row+inv)%2==0){
                System.out.println("Solution Exists!");
            }
            else {
                System.out.println("Solution doesnt Exist!!");
                return null;
            }

        }

        int initial_h_cost = heuristic_value(initial_board);
        Node node = new Node(initial_board , 0 , initial_h_cost);
        node.parent = null;
        clear_list_q();
        pq.add(node);
        visited.add(initial_board);

        node_expanded = 0;
        node_explored = 0;

        int []dx={1,-1,0,0};
        int []dy={0,0,1,-1};

        while(!pq.isEmpty()){
            node_expanded++;
            if(node_expanded % 1000000 == 0)
            {
                System.out.println(node_expanded);
            }
            Node curr = pq.remove();
            Pair p = get_blank_position(curr.board);


            int x,y;
            for(int i=0; i<4 ; i++){
                x = p.get_x() + dx[i];
                y = p.get_y() + dy[i];

                if(x<1 || x>board_size || y<1 || y>board_size)continue;

                Board child_board = curr.board.move_blank(p.get_x(), p.get_y(), x, y);


                if(visited.contains(child_board))continue;

                int child_heuristic = heuristic_value(child_board);
                Node node_child_board = new Node(child_board , curr.g_cost+1 , child_heuristic);
                node_child_board.parent = curr;
                pq.add(node_child_board);
                node_explored++;

                if(child_board.equals(final_board)){
                    System.out.print("Node Expanded : " + node_expanded + "\n");
                    System.out.println("Node Explored : " + node_explored);
                    return node_child_board;
                }
            }
            visited.add(curr.board);
        }
        System.out.println("No Solution Found!!!");
        return null;

    }

    public void print_path(Node last){
        Stack<Node> s = new Stack<>();
        while(last != null){
            s.push(last);
            last = last.parent;
        }

        while(!s.isEmpty())
        {
            Node node = s.pop();
            node.board.printBoard();;
            System.out.println("|||");
            System.out.println("|||");
            System.out.println("---");
            System.out.println("\\|/");
            
        }

    }

 }