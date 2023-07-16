import java.util.Random;

public class Heuristic {
    int h_id;

    public Heuristic(int h_id){
        this.h_id = h_id;
    }
	public int get_h_value(Game_Board bd){
        int max_player = bd.max_player;//board.getMaxPlayer();
        int min_player = (bd.max_player+1)%2;

        int stones_in_my_storage = bd.get_storage_stones( max_player );
        int stones_in_opponents_storage = bd.get_storage_stones( min_player );
        int stones_in_my_side = bd.get_total_stones( max_player );
        int stones_in_opponents_side = bd.get_total_stones( min_player );
        int max_steal_amount_diff = bd.get_max_steal_amount(max_player) - bd.get_max_steal_amount(min_player);
        int additional_move_earned = bd.bonusmoves(bd.get_current_player_id());
        int stone_captured= bd.get_max_steal_amount(max_player) - bd.get_max_steal_amount(min_player);

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

