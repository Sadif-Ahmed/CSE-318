import java.io.*;
import java.util.Random;
import java.util.Scanner;
public class Gameplay {

    private static boolean human_vs_ai=false;
	public static final boolean DEBUG = false;
	public static final int MAX_DEPTH = 15;
	private static boolean report_generation = false;
    private static int number_of_heuristics = 7;
	private static final int NO_GAMES_PER_HEURISTIC = 10;
    

    private static int ai_vs_ai_play(Heuristic h0 , Heuristic h1 , int MAX_DEPTH ) {
		Game_Board bd = new Game_Board( h0 , h1 , MAX_DEPTH );
		
        System.out.println(bd);
		int round = 0;
		while (!bd.game_over()){
			System.out.println("              Round " + round );
			int curr_player = bd.get_current_player_id();
			System.out.println("Player " + curr_player + "\'s move.");
			int bin = bd.move();
			if (bin <= 0) break;
			System.out.println("Player " + curr_player + " selects "
		                         + bd.get_moved_stones() + " stones from bin " + bin );
			System.out.println(bd);
			round++;
		}
		System.out.println( "Final board configuration:\n" );

		System.out.println( bd );
		if (bd.get_player_bin_stones( 0 , 0 ) == bd.get_player_bin_stones( 1 , 0 )) {
			System.out.println( "It's a tie!");
			return -1;
		} else if (bd.get_player_bin_stones( 0 , 0 ) > bd.get_player_bin_stones( 1 , 0 )) {
			System.out.println( "Player0 wins!" );
			return 0;
		} else {
			System.out.println( "Player1 wins!" );
			return 1;
		}
	}


    private static int human_vs_ai_play(Heuristic h0 , int MAX_DEPTH ) {
		//Heuristic s1 = new Heuristic(1);
		Game_Board bd = new Game_Board(h0 , null , MAX_DEPTH );
		
        System.out.println(bd);
		int round = 0;
		int bin;
		int curr_player;
		boolean first_ai_turn;
		Scanner sc = new Scanner(System.in);
		System.out.println("First Move by AI(1) or by you(2)?");
		int ch = sc.nextInt();
		if(ch==1){
			first_ai_turn = true;
			curr_player = 0;
			bd.set_current_palyer(0);
		}else{
			first_ai_turn=false;
			curr_player = 1;
			bd.set_current_palyer(1);
		}
		while(!bd.game_over()){
			System.out.println("              Round " + round );
			curr_player = bd.get_current_player_id();
			if(curr_player == 0)System.out.println("AI\'s move\n");
			else System.out.println("Player " + curr_player + "\'s move.");
			
			if(first_ai_turn && curr_player==0){
				bin = bd.move();
				if(bd.get_current_player_id() == 1)first_ai_turn = false;
			}
			else {
				System.out.println("Select bin[1/2/3/4/5/6](right to left)\n");
				bin = sc.nextInt();
				bd.move(bin);
				if(bd.curr_player == 0)first_ai_turn = true;
			}
			
			if (bin <= 0) break;
			if(curr_player == 0)System.out.println("AI selects " + bd.get_moved_stones() + " stones from bin " + bin);
			else System.out.println("Player " + curr_player + " selects " + bd.get_moved_stones() + " stones from bin " + bin );
			
			System.out.println(bd);
			round++;
		}
		System.out.println( "Final board configuration:\n" );

		System.out.println( bd );
		if (bd.get_player_bin_stones( 0 , 0 ) == bd.get_player_bin_stones( 1 , 0 )) {
			System.out.println( "It's a tie!" );
			return -1;
		} else if (bd.get_player_bin_stones( 0 , 0 ) > bd.get_player_bin_stones( 1 , 0 )) {
			if(human_vs_ai)System.out.println( "AI wins!" );
			else System.out.println( "Player0 wins!" );
			return 0;
		} else {
			System.out.println( "Player1 wins!" );
			return 1;
		}
	}

    public static void generate_report()throws IOException {
		PrintWriter logFile = new PrintWriter(new FileWriter("stat.log") );
		Cell [][]data = new Cell[number_of_heuristics+1][number_of_heuristics+1];
		for(int i=1 ; i<=number_of_heuristics ; i++){
			for(int j=1 ; j<=number_of_heuristics ; j++){
				data[i][j] = new Cell();
			}
		}

		int choice0;
        int choice1;
		for(int d=5 ; d<=13 ; d++){
			logFile.println("Depth "+d);
			for (int i =1;i<=number_of_heuristics;i++){
				for (int j=1;j<=number_of_heuristics;j++){
					if(i==j)continue;
				
					choice0 = i;
					choice1 = j;
					
					int p0win = 0, p1win = 0, tie = 0;
					//i first move,j 2nd move
					for (int game = 0; game <NO_GAMES_PER_HEURISTIC ; ++game) {
						int r = ai_vs_ai_play(new Heuristic(choice0) , new Heuristic(choice1) , d );
						if (r == 0) p0win++;
						if (r == 1) p1win++;
						if (r == -1) tie++;
					}
					
					logFile.println("\n\nHeuristic "+choice0+" vs "+choice1+"\nFirst Move : "+choice0);
					logFile.println( "Heuristic " + choice0 + " won : " + p0win + "\nHeuristic "
									+ choice1 + " won : " + p1win + "\nDraw : " + tie +"\n");
					logFile.flush();
					data[i][j].first_move = ((double)p0win/NO_GAMES_PER_HEURISTIC)*100.0;
					data[j][i].second_move = ((double)p1win/NO_GAMES_PER_HEURISTIC)*100.0;   										
				}
			}
			PrintWriter out=null;
			try {
				out = new PrintWriter("depth_"+d+".csv");
			} catch (Exception e) {
				//TODO: handle exception
			}
			StringBuilder sb = new StringBuilder();
			sb.append(" ,h1,h2,h3,h4,h5,h6,h7\n");
			//sb.append(" ,h1,h2\n");
			for(int p=1 ; p<=number_of_heuristics ; p++){
				sb.append("h"+p+",");
				for(int q=1 ; q<=number_of_heuristics ; q++){
					if(p==q)sb.append("-");
					else{
						sb.append("win%1st "+data[p][q].first_move
								+" win%2nd "+data[p][q].second_move);
					} 
					if(q==number_of_heuristics)sb.append(",\n");
					else sb.append(",");
				}
				sb.append("\n");
			}
			//System.out.println(sb.toString());
			out.write(sb.toString());
			out.close();
			
		}
	
	}
    public static void main(String[] args ) throws IOException {
		
		int choice0,choice1;
        Heuristic h0,h1;
		Scanner scanner = new Scanner( System.in );
		System.out.println("1.AI vs AI\nor\n2.Human vs AI\n3.Report Generation");
		int choice = scanner.nextInt();
		if (choice==2){
			//human vs AI
			human_vs_ai = true;
			System.out.println("Choose heuristic[1/2/3/4/5/6/7] for AI(player 0)");
			choice0 = scanner.nextInt();
			h0 = new Heuristic(choice0);
			human_vs_ai_play(h0, 10);
		} 
        else if(choice==3)
        {
            generate_report();
        }
		else if(choice==1){	
            System.out.println("Choose heuristic[1/2/3/4/5/6] for player 0");
            choice0 = scanner.nextInt();
            System.out.println("Choose heuristic[1/2/3/4/5/6] for player 1");
            choice1 = scanner.nextInt();
			System.out.println("Choose depth");
            int depth = scanner.nextInt();
            h0=new Heuristic(choice0);
            h1=new Heuristic(choice1);
            ai_vs_ai_play(h0 , h1 , depth);
		}
	}

}
    

