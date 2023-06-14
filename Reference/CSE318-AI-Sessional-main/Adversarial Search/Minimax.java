import java.util.ArrayList;
import java.util.Random;

public class Minimax {

	static final double INF = 10000000;
	
	
	//returns the optimal next bin idx
	public static int minimax(GameTreeNode root , int depth) {
		OptimalNode opt = alphabeta( root , -INF , INF , true , depth );
		ArrayList< GameTreeNode > list = root.successors();
		int sz = list.size();
		for (int i=0 ; i<sz ; i++){
			if(list.get(i) == null)continue;
			if(list.get(i).equals(opt.node))return i;
		}
		return -1;
	}
	
	
	public static OptimalNode alphabeta(GameTreeNode state , double alpha , double beta , boolean isMaximizing , int maxdepth ) {
		if (state.isTerminal() || maxdepth == 0){
			double h_val = state.heuristicValue();
			return new OptimalNode(state , h_val);
		}
		if (isMaximizing){
			ArrayList<OptimalNode>ls = new ArrayList<OptimalNode>();//to store all maxNodes
			OptimalNode maxNode = new OptimalNode( null , -INF );
			for (GameTreeNode s : state.successors()){
				if (s == null) continue;
				OptimalNode tmp_node = new OptimalNode(s , alphabeta(s , alpha , beta , s.isMaximizing() , maxdepth - 1 ).heuristic_value );
				maxNode = OptimalNode.max(maxNode , tmp_node);
				ls.add(maxNode);
				alpha = Math.max( alpha , maxNode.heuristic_value );
				if (alpha >= beta) break; //pruning
			}
			ArrayList<OptimalNode>best = new ArrayList<OptimalNode>();//to store best nodes if multiple
			for(int i=0 ; i<ls.size(); i++){
				if(ls.get(i).heuristic_value == maxNode.heuristic_value){
					best.add(ls.get(i));
				}
			}
			return best.get(new Random().nextInt(best.size()));//choose a best node randomly

			
		}
		else{
			OptimalNode minNode = new OptimalNode( null , INF );
			ArrayList<OptimalNode>ls = new ArrayList<OptimalNode>();
			for (GameTreeNode s : state.successors()) {
				if (s == null) continue;
				OptimalNode tmp_node = new OptimalNode( s , alphabeta( s , alpha , beta , s.isMaximizing() , maxdepth - 1 ).heuristic_value);
				minNode = OptimalNode.min(minNode , tmp_node);
				ls.add(minNode);
				beta = Math.min( beta , minNode.heuristic_value);
				if (alpha >= beta) break; //pruning
			}
			ArrayList<OptimalNode>best = new ArrayList<OptimalNode>();
			for(int i=0 ; i<ls.size(); i++){
				if(ls.get(i).heuristic_value == minNode.heuristic_value){
					best.add(ls.get(i));
				}
			}
			return best.get(new Random().nextInt(best.size()));
		}
	}
	
	
}
