import java.util.ArrayList;
import java.util.Random;
class Opt_Node {
    Tree_Node node;
    double h_value;

    Opt_Node(Tree_Node node,double h_value)
    {
        this.node=node;
        this.h_value=h_value;
    }
    public static Opt_Node find_max(Opt_Node node1,Opt_Node node2)
    {
        if(node2==null) return node1;
        else if(node1!=null && node1.h_value >= node2.h_value) return node1;
        else return node2;
    }
    public static Opt_Node find_min(Opt_Node node1,Opt_Node node2)
    {
        if(node2==null) return node1;
        else if(node1!=null && node1.h_value <= node2.h_value) return node1;
        else return node2;
    }
}

public class Minimax {
    static final double INF = 10000000;

   //returns the optimal next bin idx
	public static int mini_max(Tree_Node root , int depth) {
		Opt_Node opt = alpha_beta( root , -INF , INF , true , depth );
		ArrayList< Tree_Node > list = root.get_successor_list();
		
		for (int i=0 ; i<list.size() ; i++){
			if(list.get(i) == null)continue;
			if(list.get(i).equals(opt.node))return i;
		}
		return -1;
	}

    public static Opt_Node alpha_beta(Tree_Node state , double alpha , double beta , boolean maximizing_check , int maxdepth ) {
		if (state.terminal_check() || maxdepth == 0){
			double h_val = state.heuristic_value();
			return new Opt_Node(state , h_val);
		}
		if (maximizing_check){
			ArrayList<Opt_Node>ls = new ArrayList<Opt_Node>();//to store all maxNodes
			Opt_Node maxNode = new Opt_Node( null , -INF );
			for (Tree_Node s : state.get_successor_list()){
				if (s == null) continue;
				Opt_Node tmp_node = new Opt_Node(s , alphabeta(s , alpha , beta , s.maximizing_check() , maxdepth - 1 ).h_value );
				maxNode = Opt_Node.find_max(maxNode , tmp_node);
				ls.add(maxNode);
				alpha = Math.max( alpha , maxNode.h_value );
				if (alpha >= beta) break; //pruning
			}
			ArrayList<Opt_Node>best = new ArrayList<Opt_Node>();//to store best nodes if multiple
			for(int i=0 ; i<ls.size(); i++){
				if(ls.get(i).h_value == maxNode.h_value){
					best.add(ls.get(i));
				}
			}
			return best.get(new Random().nextInt(best.size()));//choose a best node randomly

			
		}
		else{
			Opt_Node minNode = new Opt_Node( null , INF );
			ArrayList<Opt_Node>ls = new ArrayList<Opt_Node>();
			for (Tree_Node s : state.get_successor_list()) {
				if (s == null) continue;
				Opt_Node tmp_node = new Opt_Node( s , alphabeta( s , alpha , beta , s.maximizing_check() , maxdepth - 1 ).h_value);
				minNode = Opt_Node.find_min(minNode , tmp_node);
				ls.add(minNode);
				beta = Math.min( beta , minNode.h_value);
				if (alpha >= beta) break; //pruning
			}
			ArrayList<Opt_Node>best = new ArrayList<Opt_Node>();
			for(int i=0 ; i<ls.size(); i++){
				if(ls.get(i).h_value == minNode.h_value){
					best.add(ls.get(i));
				}
			}
			return best.get(new Random().nextInt(best.size()));
		}
}
}