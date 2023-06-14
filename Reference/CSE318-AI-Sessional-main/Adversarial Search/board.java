import java.util.ArrayList;
import java.util.Arrays;
import java.util.Random;

public class board implements Cloneable,GameTreeNode{

    int[][] ara;
	
	int currentPlayer;
	int maxPlayer;
	Heuristic[] heuristics;
	int stonesMoved = 0;
	int depth;
	final boolean DEBUG = false;
	final int STORAGE = 0;//ara[i][0] ponits to the storage
    final int bins = 6;//// Bin 0 is the mancala; other bins are numbered 1 through max number.
    final int stones_per_bin = 4;
	int totalStones;
	int stole_amount0 = 0;
	int stole_amount1 = 0;
    
	
    public board(Heuristic h0 , Heuristic h1 , int depth){
        totalStones = bins * stones_per_bin * 2;
        ara = new int[2][bins + 1];
        for (int i = 0; i <= 1; i++) {
			for (int j = 1; j <= bins; j++) { 
				ara[i][j] = stones_per_bin;
			}
		}
        currentPlayer = 0; // Player0 always starts first.
		maxPlayer = 0;
		heuristics = new Heuristic[2];
		heuristics[0] = h0;
		heuristics[1] = h1;
		this.depth = depth;

    }

    public int get_total_stones_of(int player_idx) {
		int sum = 0;
		for (int j = 1; j <= bins; j++) {
			sum += ara[player_idx][j];
		}
		return sum;
	}
	
	public int stones_in_storage( int player_idx ) {
		return ara[player_idx][STORAGE];
	}

    private int getFirstNonEmptyBin(int player_idx) {
		for (int i = 1; i <= bins; i++) {
			if (ara[player_idx][i] > 0) return i;
		}
		return -1;
	}

    public int other_player(int player_idx ){
        return (player_idx + 1) % 2; 
    }
    public int opponent_player() {
		return other_player(currentPlayer);
	}
	
    public int get_bin_of_currentplayer(int bin) {
		return ara[currentPlayer][bin];
	}
	
	public int get_bin(int player_idx , int bin) {
		return ara[player_idx][bin];
	}

    public boolean can_current_player_move() {
		for (int i = 1; i <= bins; i++) {
			if (ara[currentPlayer][i] > 0) {
				return true;
			}
		}
		return false;
	}

	public int probableBonusMoves(int player_idx){
		int cnt=0;
		for(int i=1 ; i<=bins ; i++){
			if(ara[player_idx][i]==i)cnt++;
		}
		return cnt;
	}

    public boolean no_stones_in_both_sides(){
        return get_total_stones_of(currentPlayer)==0 && get_total_stones_of(opponent_player())==0;
    }

    public boolean is_game_over(){
        return no_stones_in_both_sides();
    }

	public int countTotalStones() {
		int result = 0;
		for (int i = 0; i <= 1; i++) {
			for (int j = 0; j <= bins; j++) {
				result = result + ara[i][j];
			}
		}
		return result;
	}

	private void captureBin(int player_idx , int bin ) {
		int oppositeBin = bins + 1 - bin;
		int oppositePlayer = other_player(player_idx);
		ara[player_idx][STORAGE] += (ara[oppositePlayer][oppositeBin] + 1);
		ara[oppositePlayer][oppositeBin] = 0;
	}
	
	private void moveToMancala( int player_idx){
		for (int i = 1; i <= bins; i++){
			stonesMoved = ara[player_idx][i];
			ara[player_idx][STORAGE] += ara[player_idx][i];
			ara[player_idx][i] = 0;
		}
	}

	public int is_steal_move(int player_idx , int bin){
		int stones = ara[player_idx][bin];
		if(stones==0)return -1;
		int curr_bin = bin - 1;
		int curr_side = player_idx;
		for(int s=stones ; s>0 ; s--){
			if ((s == 1) && (curr_side == currentPlayer) && (curr_bin > 0)
						    && (ara[curr_side][curr_bin] == 0)){
						return curr_bin;
			}
			
			if(curr_bin==0){
				curr_side = other_player(curr_side);
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
			if(is_steal_move(player_idx, i)>0){
				int oppositeBin = bins + 1 - is_steal_move(player_idx, i);
				int oppositePlayer = other_player(player_idx);
				mx = Math.max(mx , ara[oppositePlayer][oppositeBin]);
			}
		}
		return mx;
	}

	public int get_weighted_stones_sum(int player_idx){
		int sum = 0 , wt;
		//giving larger weights to bins close to the storage		
		for(int i=1 ; i<=bins ; i++){	
			wt=i;
			sum += wt*ara[player_idx][i];
		}
		return sum;
		
	}

	private void setCurrentPlayer(int currentPlayer){
		this.currentPlayer = currentPlayer;
	}
	
	private void setMaxPlayer(int maxPlayer){
		this.maxPlayer = maxPlayer;
	}

	int move() {
		int bin = getFirstNonEmptyBin( currentPlayer);
		if (get_total_stones_of(opponent_player()) != 0) { 
			this.setMaxPlayer( currentPlayer );
			bin = heuristics[currentPlayer].selectMove( this , depth );
		}
		
		move( bin );
		return bin;
	}
	
	
	void move(int bin){		
		int stones = ara[currentPlayer][bin];
		if (stones == 0) {
			System.err.println( "Error" );
			stonesMoved = stones;
			return;
		}
		if (get_total_stones_of(opponent_player()) == 0) {
			moveToMancala( currentPlayer );
		} else {
			stonesMoved = stones;
			ara[currentPlayer][bin] = 0;
			int currentSide = currentPlayer;
			int currentBin = bin - 1; 
			for (int s = stones; s > 0; s--){
				//capturing options
				if ((s == 1) && (currentSide == currentPlayer) && (currentBin > 0) && (ara[currentSide][currentBin] == 0)) {
					captureBin( currentPlayer , currentBin );
				}else if (currentBin == 0) {
					if (currentSide == currentPlayer) {
						// If it's our mancala, place a stone.
						ara[currentSide][currentBin]++;
						if (s == 1) {
							// We're placing our last stone in our own MancalaBoard.
							// so currentplayer will get a bonus turn and thus we return
							// if not returned from here , opponentplayer will be set as 
							// currentplayer after this loop ends. 
							if (!can_current_player_move()) {
								moveToMancala(other_player(currentPlayer ) );
							}
							return;
						}
					} else {
						// If it's other mancala, don't place a stone.
						s++;
					}
					//In any case, switch to other side.
					currentSide = other_player( currentSide );
					currentBin = bins;
				} else {
					// Regular stone distribution.
					ara[currentSide][currentBin]++;
					currentBin--;
				}
			}
			setCurrentPlayer(opponent_player());
			if (!can_current_player_move()){
				moveToMancala(other_player(currentPlayer));
			}
		}
	}

		
	@Override
	protected Object clone() throws CloneNotSupportedException {
		board clone = (board) super.clone();
		clone.ara = new int[this.ara.length][this.ara[0].length];
		for (int r = 0; r < this.ara.length; r++){
			if (this.ara[r].length >= 0){
				System.arraycopy( this.ara[r] , 0 , clone.ara[r] , 0 , this.ara[r].length );
			}
		}
		return clone;
	}
	
    @Override
	public double heuristicValue(){
		return heuristics[maxPlayer].getHeuristicValue(this);
	}
	
	public board getSuccessor( int bin ) throws CloneNotSupportedException {
		board bd = (board) this.clone();
		bd.move( bin );
		return bd;
	}
	
	@Override
	public ArrayList< GameTreeNode > successors() {
		ArrayList< GameTreeNode > suclist = new ArrayList<>();
		for (int i = 1; i <= bins; ++i) {
			try {
				if(ara[currentPlayer][i] > 0)
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
	public boolean isTerminal() {
		return is_game_over();
	}
	
	@Override
	public boolean isMaximizing() {
		return currentPlayer == maxPlayer;
	}
	
	@Override
	public boolean equals( GameTreeNode o ) {
		if (this == o) return true;
		if (o == null) return false;
		board mancalaBoard = (board) o;
		return bins == mancalaBoard.bins &&
				       totalStones == mancalaBoard.totalStones &&
				       currentPlayer == mancalaBoard.currentPlayer &&
				       stonesMoved == mancalaBoard.stonesMoved &&
				       Arrays.deepEquals( ara , mancalaBoard.ara );
	}

	public String toString(){
		ConsolePrint cp = new ConsolePrint(this.ara);
		return cp.edgeLine()+
			   cp.player0Line()+
			   cp.middleLine()+
			   cp.player1Line()+
			   cp.edgeLine();
	}
	
}