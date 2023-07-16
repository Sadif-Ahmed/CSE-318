import java.io.*;
import java.util.Random;
import java.util.Scanner;
public class Gameplay {

	public static final int MAX_DEPTH = 15;
	private static int number_of_heuristics = 7;
	private static final int GAMES_PER_HEURISTIC = 10;
	
	private static final Scanner input = new Scanner(System.in);

    private static int ai_vs_ai_play(int h0 , int h1 , int MAX_DEPTH ) {
		Game_Board board = new Game_Board( h0 , h1 , MAX_DEPTH );
		
        board.print_board();
		int round = 0;
		while (!board.game_over()){
			System.out.println("              Round " + round );
			int curr_player = board.get_current_player_id();
			System.out.println("Player " + curr_player + "\'s move.");
			int bin = board.move();
			if (bin <= 0) break;
			System.out.println("Player " + curr_player + " selects "
		                         + board.get_moved_stones() + " stones from bin " + bin );
			board.print_board();
			round++;
		}
		System.out.println( "Final board configuration:\n" );

		board.print_board();
		return result_calculation(board);
		
	}
	private static int  human_vs_human_play()
	{
		Game_Board board = new Game_Board( 0, 0, MAX_DEPTH);
		int bin;

		board.print_board();

		while(!board.game_over())
		{
				System.out.println("Player"+board.curr_player+"move:");
			    System.out.println("Select bin[1/2/3/4/5/6](right to left)\n");
				bin = input.nextInt();
				board.move(bin);
				System.out.println("Player" + board.get_current_player_id() + " selects " + board.get_moved_stones() + " stones from bin " + bin );
				board.print_board();
		}
		System.out.println("Final Board Configuration : ");
		board.print_board();

		return result_calculation(board);
		

	}
    private static int human_vs_ai_play(int h0 , int MAX_DEPTH ) {
		
		Game_Board board = new Game_Board(h0 , 0 , MAX_DEPTH );
		
        board.print_board();
		int round = 0;
		int bin;
		int curr_player;
		boolean first_ai_turn;
		System.out.println("First Move by AI(1) or by you(2)?");
		int ch = input.nextInt();
		if(ch==1){
			first_ai_turn = true;
			curr_player = 0;
			board.set_current_palyer(0);
		}else{
			first_ai_turn=false;
			curr_player = 1;
			board.set_current_palyer(1);
		}
		while(!board.game_over()){
			System.out.println("              Round " + round );
			curr_player = board.get_current_player_id();
			if(curr_player == 0)System.out.println("AI\'s move\n");
			else System.out.println("Player " + curr_player + "\'s move.");
			
			if(first_ai_turn && curr_player==0){
				bin = board.move();
				if(board.get_current_player_id() == 1)first_ai_turn = false;
			}
			else {
				System.out.println("Select bin[1/2/3/4/5/6](right to left)\n");
				bin = input.nextInt();
				board.move(bin);
				if(board.curr_player == 0)first_ai_turn = true;
			}
			
			if (bin <= 0) break;
			if(curr_player == 0)System.out.println("AI selects " + board.get_moved_stones() + " stones from bin " + bin);
			else System.out.println("Player " + curr_player + " selects " + board.get_moved_stones() + " stones from bin " + bin );
			
			board.print_board();
			round++;
		}
		System.out.println( "Final board configuration:\n" );

		board.print_board();
		int result = result_calculation(board);
		if(result==0)
		{
			System.out.println( "AI wins!" );
		} 
		return result;
	}
	private static int result_calculation(Game_Board board)
	{
		if (board.get_storage_stones(0) == board.get_storage_stones(1)) {
			System.out.println( "It's a tie!");
			return -1;
		} else if (board.get_storage_stones(0) > board.get_storage_stones(1)) {
			System.out.println( "Player0 wins!" );
			return 0;
		} else {
			System.out.println( "Player1 wins!" );
			return 1;
		}
	}
	public static void genearte_csv(Cell[][] data,int depth)
	{
		
		
				PrintWriter out=null;
			try {
				out = new PrintWriter("depth "+depth+".csv");
			} catch (Exception e) {
				//TODO: handle exception
			}
			StringBuilder sb = new StringBuilder();
			sb.append(" ,h1,h2,h3,h4,h5,h6,h7\n");
			//sb.append(" ,h1,h2\n");
			for(int i=1 ; i<=number_of_heuristics ; i++){
				sb.append("h"+i+",");
				for(int j=1 ; j<=number_of_heuristics ; j++){
					if(i==j)sb.append("-");
					else{
						sb.append("win%1st "+data[i][j].first_move
								+" win%2nd "+data[i][j].second_move);
					} 
					if(j==number_of_heuristics)sb.append(",\n");
					else sb.append(",");
				}
				sb.append("\n");
			}
			out.write(sb.toString());
			out.close();
			}
	

    public static void generate_report()throws IOException {
		PrintWriter logFile = new PrintWriter(new FileWriter("stat.log") );
		Cell [][]data = new Cell[number_of_heuristics+1][number_of_heuristics+1];
		for(int i=1 ; i<=number_of_heuristics ; i++){
			for(int j=1 ; j<=number_of_heuristics ; j++){
				data[i][j] = new Cell();
			}
		}
		for(int d=5 ; d<=13 ; d++){
			logFile.println("Depth "+d);
			for (int i =1;i<=number_of_heuristics;i++){
				for (int j=1;j<=number_of_heuristics;j++){
					if(i==j)continue;
					
					int p0win = 0, p1win = 0, tie = 0;
					//i first move,j 2nd move
					for (int game = 0; game <GAMES_PER_HEURISTIC ; ++game) {
						int result = ai_vs_ai_play(i , j , d );
						if (result == 0) p0win++;
						if (result == 1) p1win++;
						if (result == -1) tie++;
					}
					
					logFile.println("\n\nHeuristic "+i+" vs "+j+"\nFirst Move : "+i);
					logFile.println( "Heuristic " + i+ " won : " + p0win + "\nHeuristic "
									+ j + " won : " + p1win + "\nDraw : " + tie +"\n");
					logFile.flush();
					data[i][j].first_move = ((double)p0win/GAMES_PER_HEURISTIC)*100.0;
					data[j][i].second_move = ((double)p1win/GAMES_PER_HEURISTIC)*100.0;   										
				}
			}
			genearte_csv(data, d);
			
		}
	
	}
    public static void main(String[] args ) throws IOException {
		
		int choice0,choice1;
		System.out.println("1.AI vs AI\n2.Human vs AI\n3.Human vs Human\n4.Report Generation");
		int choice = input.nextInt();
		if (choice==2){
			//human vs AI
			System.out.println("Choose heuristic[1/2/3/4/5/6/7] for AI(player 0)");
			choice0 = input.nextInt();
			human_vs_ai_play(choice0, 10);
		} 
		else if(choice==3)
		{
			human_vs_human_play();
		}
        else if(choice==4)
        {
            generate_report();
        }
		else if(choice==1){	
            System.out.println("Choose heuristic[1/2/3/4/5/6/7] for player 0");
            choice0 = input.nextInt();
            System.out.println("Choose heuristic[1/2/3/4/5/6/7] for player 1");
            choice1 = input.nextInt();
			System.out.println("Choose depth");
            int depth = input.nextInt();
            ai_vs_ai_play(choice0 , choice1 , depth);
		}
	}

}
    

