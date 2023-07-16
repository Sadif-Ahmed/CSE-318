
import java.util.*;

public class Game_Board implements Cloneable,Tree_Node {

	//Board Storage
    int [][] mat;
	//Players
    int curr_player;
    int max_player;
	//Heuristics
    Heuristic choice0;
    Heuristic choice1;
	//Stone Counts
    int moved_stones=0;
    int total_stones;
    int stole_amount0=0;
    int stole_amount1=0;
	//Depth
    int depth;

	final int STORAGE = 0;//mat[i][0] points to the storage
    final int bins = 6;//// Bin 0 is the mancala; other bins are numbered 1 through max number.
    final int stones_per_bin = 4;

	private final String RESET = "\u001B[0m";
	private final String GREEN = "\u001B[32m";
	private final String YELLOW = "\u001B[33m";
	private final String BLUE = "\u001B[34m";
	private final String PURPLE = "\u001B[35m";

    public Game_Board(Heuristic h0,Heuristic h1,int depth)
    {
        total_stones= bins * stones_per_bin * 2 ;
        mat = new int[2][bins+1];
        for (int i = 0; i <= 1; i++) {
			for (int j = 1; j <= bins; j++) { 
				mat[i][j] = stones_per_bin;
			}
		}
        curr_player = 0;
        max_player = 0;
        this.choice0=h0;
        this.choice1=h1;
        this.depth=depth;
    }

    public int get_total_stones(int player_id)
    {
        int ret=0;
        for(int i=1;i<=bins;i++)
        {
            ret+=mat[player_id][i];
        }
        return ret;
    }
    public int get_storage_stones(int player_id)
    {
        return mat[player_id][STORAGE];
    }
    public int get_first_nonempty_bin(int player_id)
    {
        for(int i=1;i<=bins;i++)
        {
            if(mat[player_id][i]!=0)
            {
                return i;
            }
        }
        return -1;
    }
    public int get_opponent_player_id()
    {
        return (curr_player+1)%2;
    }
	public int get_current_player_id()
    {
        return curr_player;
    }
	public int get_moved_stones()
	{
		return moved_stones;
	}
    public int get_curr_player_bin_stones(int bin)
    {
        return mat[curr_player][bin];
    }
    public int get_player_bin_stones(int player_id,int bin)
    {
        return mat[player_id][bin];
    }
    public boolean can_curr_player_play()
    {
        for(int i=1;i<=bins;i++)
        {
            if(mat[curr_player][i]>0)
            {
                return true;
            }
        }
        return false;
    }
    public int bonusmoves(int player_id)
    {
        int ret=0;
        for(int i=1;i<=bins;i++)
        {
            if(mat[player_id][i]==i)
            {
                ret++;
            }
        }
        return ret;
    }
    public boolean game_over()
    {
        if(get_total_stones(curr_player)==0 && get_total_stones(get_opponent_player_id())==0)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    public int totalstones_count() {
		int ret = 0;
		for (int i = 0; i <= 1; i++) {
			for (int j = 0; j <= bins; j++) {
				ret = ret + mat[i][j];
			}
		}
		return ret;
	}
    
    private void capt_bin(int player_id,int bin)
    {
        int opposite_bin = bins + 1 - bin;
        int opposite_player = (player_id+1)%2;
        mat[player_id][STORAGE] += (mat[opposite_player][opposite_bin] + 1);
        mat[opposite_player][opposite_bin] = 0;

    }
    private void move_to_mancala( int player_id){
		for (int i = 1; i <= bins; i++){
			moved_stones = mat[player_id][i];
			mat[player_id][STORAGE] += mat[player_id][i];
			mat[player_id][i] = 0;
		}
	}
    public int steal_move_check(int player_idx , int bin){
		int stones = mat[player_idx][bin];
		if(stones==0)return -1;
		int curr_bin = bin - 1;
		int curr_side = player_idx;
		for(int s=stones ; s>0 ; s--){
			if ((s == 1) && (curr_side == curr_player) && (curr_bin > 0)
						    && (mat[curr_side][curr_bin] == 0)){
						return curr_bin;
			}
			
			if(curr_bin==0){
				curr_side = (curr_side+1)%2;
				curr_bin = bins;
				continue;
			}
			curr_bin--;
		}
		return -1;
	}
    public int get_max_steal_amount(int player_idx){
		int mx = 0;
		for(int i=1;i<=bins;i++){
			//chk if move from this bin is a steal move
			if(steal_move_check(player_idx, i)>0){
				int oppositeBin = bins + 1 - steal_move_check(player_idx, i);
				int oppositePlayer = (player_idx+1)%2;
				mx = Math.max(mx , mat[oppositePlayer][oppositeBin]);
			}
		}
		return mx;
	}
    public void set_current_palyer(int curr_player)
    {
        this.curr_player=curr_player;
    }
    private void set_max_player(int max_player)
    {
        this.max_player=max_player;
    }
    int move() {
		int bin = get_first_nonempty_bin( curr_player);
		if (get_total_stones(get_opponent_player_id()) != 0) { 
			this.set_max_player( curr_player );
            if(curr_player==0)
            {
                bin = choice0.select_move( this , depth );
            }
			else
            {
                bin = choice1.select_move(this,depth);
            }
		}
		move( bin );
		return bin;
	}

    void move(int bin){		
		int stones = mat[curr_player][bin];
		if (stones == 0) {
			System.err.println( "Error" );
			moved_stones = stones;
			return;
		}
		if (get_total_stones((curr_player+1)%2) == 0) {
			move_to_mancala( curr_player );
		} else {
			moved_stones = stones;
			mat[curr_player][bin] = 0;
			int currentSide = curr_player;
			int currentBin = bin - 1; 
			for (int s = stones; s > 0; s--){
				//capturing options
				if ((s == 1) && (currentSide == curr_player) && (currentBin > 0) && (mat[currentSide][currentBin] == 0)) {
					capt_bin( curr_player , currentBin );
				}else if (currentBin == 0) {
					if (currentSide == curr_player) {
						// If it's our mancala, place a stone.
						mat[currentSide][currentBin]++;
						if (s == 1) {
							// We're placing our last stone in our own MancalaBoard.
							// so currentplayer will get a bonus turn and thus we return
							// if not returned from here , opponentplayer will be set as 
							// currentplayer after this loop ends. 
							if (!can_curr_player_play()) {
								move_to_mancala((curr_player+1)%2);
							}
							return;
						}
					} else {
						// If it's other mancala, don't place a stone.
						s++;
					}
					//In any case, switch to other side.
					currentSide = (currentSide+1)%2;
					currentBin = bins;
				} else {
					// Regular stone distribution.
					mat[currentSide][currentBin]++;
					currentBin--;
				}
			}
			set_current_palyer((curr_player+1)%2);
			if (!can_curr_player_play()){
				move_to_mancala((curr_player+1)%2);
			}
		}
	}
    @Override
	protected Object clone() throws CloneNotSupportedException {
		Game_Board clone = (Game_Board) super.clone();
		clone.mat = new int[this.mat.length][this.mat[0].length];
		for(int i=0;i<2;i++)
		{
			for(int j=0;j<=6;j++)
			{
				clone.mat[i][j]=this.mat[i][j];
			}
		}
		return clone;
	}
	
    @Override
	public double heuristic_value(){
        if(max_player==0)
        {
            return choice0.get_h_value(this);
        }
        else
        {
            return choice1.get_h_value(this);
        }
		
	}
	
	public Game_Board getSuccessor( int bin ) throws CloneNotSupportedException {
		Game_Board bd = (Game_Board) this.clone();
		bd.move( bin );
		return bd;
	}
	
	@Override
	public ArrayList <Tree_Node> get_successor_list() {
		ArrayList< Tree_Node > suclist = new ArrayList<>();
		for (int i = 1; i <= bins; ++i) {
			try {
				if(mat[curr_player][i] > 0)
					suclist.add( i - 1 , getSuccessor( i ) );
				else
					suclist.add( i - 1 , null );
			} catch (CloneNotSupportedException e) {
				e.printStackTrace();
			}
		}
		return suclist;
	}
	
	@Override
	public boolean terminal_check() {
		return game_over();
	}
	
	@Override
	public boolean maximizing_check() {
		return curr_player == max_player;
	}
	
	@Override
	public boolean equals( Tree_Node o ) {
		if (this == o) return true;
		if (o == null) return false;
		Game_Board temp = (Game_Board) o;
		return bins == temp.bins &&
				       total_stones == temp.total_stones &&
				       curr_player == temp.curr_player &&
				       moved_stones == temp.moved_stones &&
				       Arrays.deepEquals( mat , temp.mat );
	}

	public String edgeLine() {
		return YELLOW + "+----" + middleDashes() + "----+\n" + RESET;
	}
    public String player0Line() {
		StringBuilder sb = new StringBuilder();
		sb.append(YELLOW+ "|    |" +RESET);
		for (int i = 1; i <= bins; i++){
			sb.append( " " ).append(GREEN + numberString(mat[0][i]) + RESET ).append(YELLOW+ " |" + RESET);
		}
		sb.append(YELLOW + "    |\n" +RESET);
		return sb.toString();
	}
    public String middleLine() {
		return YELLOW+"| " +RESET+ PURPLE + numberString(mat[0][0]) + RESET + " "
				       + YELLOW+ middleDashes()+RESET
				       + " " + PURPLE + numberString(mat[1][0]) + RESET + YELLOW+" |\n"+RESET;
	}
    public String player1Line() {
		StringBuilder sb = new StringBuilder();
		sb.append(YELLOW+ "|    |" +RESET);
		for (int i = bins; i > 0; i--) {
			sb.append( " " ).append(BLUE + numberString(mat[1][i]) + RESET ).append( YELLOW+ " |" + RESET);
		}
		sb.append( YELLOW + "    |\n" +RESET);
		return sb.toString();
	}
    public String middleDashes() {
		StringBuilder sb = new StringBuilder();
		for (int i = 1; i <= bins; i++) {
			sb.append( "+----" );
		}
		sb.append( "+" );
		return sb.toString();
	}

    public String numberString( int n ) {
		if ((0 <= n) && (n < 10)) {
			return " " + n;
		} else {
			return Integer.toString( n );
		}
	}
	public void print_board()
	{
		       String temp = edgeLine()+player0Line()+middleLine()+player1Line()+edgeLine(); 
			   System.out.println(temp);
	}

    






}
