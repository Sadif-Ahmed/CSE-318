import java.util.Random;

public class Heuristic {
    int h_id;

    public Heuristic(int h_id){
        this.h_id = h_id;
    }

	public int select_move( Game_Board bd , int depth ){
		int bin = 0;
		try {
			bin = Minimax.mini_max( bd , depth ) + 1; 
		} catch (Exception e) {
			e.printStackTrace();
		}
		return bin;
	}
	
	
	
	
	
	public int get_h_value(Game_Board bd){
        int maxPlayer = bd.max_player;//board.getMaxPlayer();
        int minPlayer = (bd.max_player+1)%2;

        int stones_in_my_storage = bd.get_storage_stones( maxPlayer );
        int stones_in_opponents_storage = bd.get_storage_stones( minPlayer );
        int stones_in_my_side = bd.get_total_stones( maxPlayer );
        int stones_in_opponents_side = bd.get_total_stones( minPlayer );

        int additional_move_earned = bd.bonusmoves(bd.get_current_player_id());

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
        else if(h_id==4)
        {
            int W1 = new Random().nextInt( 10 ) + 1;
            int W2 = new Random().nextInt( 6 ) + 1;
            int W3 = new Random().nextInt( 5 ) + 1;
            int W4 = new Random().nextInt( 7 ) + 1;

            int stone_captured= bd.get_max_steal_amount(maxPlayer) - bd.get_max_steal_amount(minPlayer);
            return W1*(stones_in_my_storage-stones_in_opponents_storage) 
            + W2*(stones_in_my_side-stones_in_opponents_side) 
            + W3*additional_move_earned 
            + W4*stone_captured;
        }
        else if(h_id==5){

            int W1 = new Random().nextInt( 10 ) + 1;
            int W2 = new Random().nextInt( 6 ) + 1;
            
            int max_steal_amount_diff = bd.get_max_steal_amount(maxPlayer) - bd.get_max_steal_amount(minPlayer);

            return W1 * (4*stones_in_my_storage - 2*stones_in_opponents_storage) 
            + W2*max_steal_amount_diff;
        }
        else if(h_id == 6){
            int W1 = new Random().nextInt( 10 ) + 1;
            int W3 = new Random().nextInt( 6 ) + 1;
            int W4 = new Random().nextInt( 8 ) + 1;

            int max_steal_amount_diff = bd.get_max_steal_amount(maxPlayer) - bd.get_max_steal_amount(minPlayer);

            return W1 * (4*stones_in_my_storage - 2*stones_in_opponents_storage) 
                     + W3*additional_move_earned
                     + W4*max_steal_amount_diff;
        }
        else if(h_id == 7){
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

