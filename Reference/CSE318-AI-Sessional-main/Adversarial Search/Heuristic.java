import java.util.Random;

public class Heuristic {
    int h_id;

    public Heuristic(int h_id){
        this.h_id = h_id;
    }

	public int selectMove( board bd , int depth ){
		int bin = 0;
		try {
			bin = Minimax.minimax( bd , depth ) + 1; 
		} catch (Exception e) {
			e.printStackTrace();
		}
		return bin;
	}
	
	
	public static Heuristic h_idToStrategy( int i ) {
        if(i==1)return new Heuristic(1);
        else if(i==2)return new Heuristic(2);
        else if(i==3)return new Heuristic(3);
        else if(i==4)return new Heuristic(4);
        else if(i==5)return new Heuristic(5);
        else if(i==6)return new Heuristic(6);
        else return new Heuristic(3);//default heuristic 3
    }
	
	
	public int getHeuristicValue(board bd){
        int maxPlayer = bd.maxPlayer;//board.getMaxPlayer();
        int minPlayer = bd.other_player( maxPlayer );

        int stones_in_my_storage = bd.stones_in_storage( maxPlayer );
        int stones_in_opponents_storage = bd.stones_in_storage( minPlayer );
        int stones_in_my_side = bd.get_total_stones_of( maxPlayer );
        int stones_in_opponents_side = bd.get_total_stones_of( minPlayer );

        int additional_move_earned = bd.probableBonusMoves(bd.currentPlayer);

        if(h_id==1){        
            //heuristic-1: The evaluation function is
            //(stones_in_my_storage – stones_in_opponents_storage)
            return stones_in_my_storage - stones_in_opponents_storage;
        }
        else if(h_id==2){
            int W1 = new Random().nextInt( 10 ) + 1;
            int W2 = new Random().nextInt( 5 ) + 1;
            //W1 * (stones_in_my_storage – stones_in_opponents_storage) + W2 * (stones_on_my_side –
            //stones_on_opponents_side)
            return W1 * (stones_in_my_storage - stones_in_opponents_storage) + W2 * (stones_in_my_side - stones_in_opponents_side);
    
        }
        else if(h_id==3){
            int W1 = new Random().nextInt( 10 ) + 1;
            int W2 = new Random().nextInt( 6 ) + 1;
            int W3 = new Random().nextInt( 5 ) + 1;
            /*
            Heuristic-3: The evaluation function is
            W1 * (stones_in_my_storage – stones_in_opponents_storage) + W2 * (stones_on_my_side –
            stones_on_opponents_side) + W3 * (additional_move_earned)
            */
            return W1 * (stones_in_my_storage - stones_in_opponents_storage) + W2 * (stones_in_my_side - stones_in_opponents_side) + W3 * additional_move_earned;
        }
        else if(h_id==4){

            int W1 = new Random().nextInt( 10 ) + 1;
            int W2 = new Random().nextInt( 6 ) + 1;
            
            int max_steal_amount_diff = bd.get_max_steal_amount(maxPlayer) - bd.get_max_steal_amount(minPlayer);

            return W1 * (4*stones_in_my_storage - 2*stones_in_opponents_storage) + W2*max_steal_amount_diff;
        }
        else if(h_id == 5){
            int W1 = new Random().nextInt( 10 ) + 1;
            int W3 = new Random().nextInt( 6 ) + 1;
            int W4 = new Random().nextInt( 8 ) + 1;

            int max_steal_amount_diff = bd.get_max_steal_amount(maxPlayer) - bd.get_max_steal_amount(minPlayer);

            return W1 * (4*stones_in_my_storage - 2*stones_in_opponents_storage) 
                     + W3*additional_move_earned
                     + W4*max_steal_amount_diff;
        }
        else if(h_id == 6){
            int max_steal_amount_diff = bd.get_max_steal_amount(maxPlayer) - bd.get_max_steal_amount(minPlayer);
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
