import java.util.ArrayList;
import java.util.Random;
public class Algo_Heuristics {
    Tree_Node node;
    double h_value;
    static final double INF = 10000000;

    Algo_Heuristics(Tree_Node node,double h_value)
    {
        this.node=node;
        this.h_value=h_value;
    }
    public static Algo_Heuristics find_max(Algo_Heuristics node1,Algo_Heuristics node2)
    {
        if(node2==null) return node1;
        else if(node1!=null && node1.h_value >= node2.h_value) return node1;
        else return node2;
    }
    public static Algo_Heuristics find_min(Algo_Heuristics node1,Algo_Heuristics node2)
    {
        if(node2==null) return node1;
        else if(node1!=null && node1.h_value <= node2.h_value) return node1;
        else return node2;
    }


   //returns the optimal next bin idx
	public static int mini_max(Tree_Node root , int depth) {
		Algo_Heuristics opt = alpha_beta( root , -INF , INF , true , depth );
		ArrayList< Tree_Node > list = root.get_successor_list();
		
		for (int i=0 ; i<list.size() ; i++){
			if(list.get(i) == null)continue;
			if(list.get(i).equals(opt.node))return i;
		}
		return -1;
	}
    public static ArrayList<Algo_Heuristics> find_best(ArrayList<Algo_Heuristics> candidates,Algo_Heuristics reference)
    {
        ArrayList<Algo_Heuristics> bestlist = new ArrayList<Algo_Heuristics>();
        for(int i=0;i<candidates.size();i++)
        {
            if(candidates.get(i).h_value==reference.h_value)
            {
                bestlist.add(candidates.get(i));
            }
        }
        return bestlist;
    }

    public static Algo_Heuristics alpha_beta(Tree_Node state , double alpha , double beta , boolean maximizing_check , int maxdepth ) {
		if (state.terminal_check() || maxdepth == 0){
			double h_val = state.heuristic_value();
			return new Algo_Heuristics(state , h_val);
		}
		if (maximizing_check){
			ArrayList<Algo_Heuristics>max_node_list = new ArrayList<Algo_Heuristics>();//to store all maxNodes
			Algo_Heuristics max_node = new Algo_Heuristics( null , -INF );
			for (Tree_Node s : state.get_successor_list()){
				if (s == null) continue;
				Algo_Heuristics tmp_node = new Algo_Heuristics(s , alpha_beta(s , alpha , beta , s.maximizing_check() , maxdepth - 1 ).h_value );
				max_node = find_max(max_node , tmp_node);
				max_node_list.add(max_node);
				alpha = Math.max( alpha , max_node.h_value );
				if (alpha >= beta) break; //pruning
			}
			ArrayList<Algo_Heuristics>best_list = new ArrayList<Algo_Heuristics>();//to store best nodes if multiple
			best_list=find_best(max_node_list, max_node);
			return best_list.get(new Random().nextInt(best_list.size()));//choose a best node randomly	
		}
		else{
			ArrayList<Algo_Heuristics>min_node_list = new ArrayList<Algo_Heuristics>();
            Algo_Heuristics min_node = new Algo_Heuristics( null , INF );
			for (Tree_Node s : state.get_successor_list()) {
				if (s == null) continue;
				Algo_Heuristics tmp_node = new Algo_Heuristics( s , alpha_beta( s , alpha , beta , s.maximizing_check() , maxdepth - 1 ).h_value);
				min_node = find_min(min_node , tmp_node);
				min_node_list.add(min_node);
				beta = Math.min( beta , min_node.h_value);
				if (alpha >= beta) break; //pruning
			}
			ArrayList<Algo_Heuristics>best_list = new ArrayList<Algo_Heuristics>();//to store best nodes if multiple
			best_list=find_best(min_node_list, min_node);
			return best_list.get(new Random().nextInt(best_list.size()));
		}
}
public static int get_h_value(Game_Board bd,int h_id){
        int max_player = bd.max_player;
        int min_player = (bd.max_player+1)%2;

        int stones_in_my_storage = bd.get_storage_stones( max_player );
        int stones_in_opponents_storage = bd.get_storage_stones( min_player );
        int stones_in_my_side = bd.get_total_stones( max_player );
        int stones_in_opponents_side = bd.get_total_stones( min_player );
        int max_steal_amount_diff = bd.get_max_steal_amount(max_player) - bd.get_max_steal_amount(min_player);
        int additional_move_earned = bd.bonusmoves(bd.get_current_player_id());
        int stone_captured= bd.get_max_steal_amount(bd.get_current_player_id());
        if(h_id==1){        
            return stones_in_my_storage - stones_in_opponents_storage;
        }
        else if(h_id==2){
            int W1 = new Random().nextInt( 10 ) + 1;
            int W2 = new Random().nextInt( 5 ) + 1;
            return W1 * (stones_in_my_storage - stones_in_opponents_storage) + W2 * (stones_in_my_side - stones_in_opponents_side);
    
        }
        else if(h_id==3){
            int W1 = new Random().nextInt( 10 ) + 1;
            int W2 = new Random().nextInt( 6 ) + 1;
            int W3 = new Random().nextInt( 5 ) + 1;
            return W1 * (stones_in_my_storage - stones_in_opponents_storage) + W2 * (stones_in_my_side - stones_in_opponents_side) + W3 * additional_move_earned;
        }
        else if(h_id==4)
        {
            int W1 = new Random().nextInt( 10 ) + 1;
            int W2 = new Random().nextInt( 6 ) + 1;
            int W3 = new Random().nextInt( 5 ) + 1;
            int W4 = new Random().nextInt( 7 ) + 1;

            
            return W1*(stones_in_my_storage-stones_in_opponents_storage) 
            + W2*(stones_in_my_side-stones_in_opponents_side) 
            + W3*additional_move_earned 
            + W4*stone_captured;
        }
        else if(h_id==5){

            int W1 = new Random().nextInt( 10 ) + 1;
            int W2 = new Random().nextInt( 6 ) + 1;

            return W1 * (4*stones_in_my_storage - 2*stones_in_opponents_storage) 
            + W2*max_steal_amount_diff;
        }
        else if(h_id == 6){
            int W1 = new Random().nextInt( 10 ) + 1;
            int W3 = new Random().nextInt( 6 ) + 1;
            int W4 = new Random().nextInt( 8 ) + 1;

            
            return W1 * (4*stones_in_my_storage - 2*stones_in_opponents_storage) 
                     + W3*additional_move_earned
                     + W4*max_steal_amount_diff;
        }
        else if(h_id == 7){
            
            int W1 = new Random().nextInt( 10 ) + 1;
            int W2 = new Random().nextInt( 5 ) + 1;
            int W3 = new Random().nextInt( 5 ) + 1;
            int W4 = new Random().nextInt( 5 ) + 1;
            return W1 * (4*stones_in_my_storage - 2*stones_in_opponents_storage) 
                        + W2 * (stones_in_my_side - stones_in_opponents_side) 
                        + W3 * additional_move_earned
                        + W4 * max_steal_amount_diff;

        }
        else return -1;
        
    }
}