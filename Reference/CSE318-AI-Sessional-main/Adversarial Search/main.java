import java.io.*;
import java.util.Random;
import java.util.Scanner;

public class main{
	private static boolean human_vs_AI=false;
	public static final boolean DEBUG = false;
	public static final int MAX_DEPTH = 15;
	private static boolean generate_report = false;
	private static final int[] h_list0 = { 1 , 2 ,3 ,4 , 5 , 6};
	private static final int[] h_list1 = { 1 , 2 ,3 ,4 , 5 , 6};
	private static final int NO_GAMES_PER_HEURISTIC = 10;

	private static Heuristic selectStrategy(int n){
		return Heuristic.h_idToStrategy(n);
	}
	
	
	private static int play(Heuristic s0 , Heuristic s1 , int MAX_DEPTH ) {
		board bd = new board( s0 , s1 , MAX_DEPTH );
		
        System.out.println(bd);
		int round = 0;
		while (!bd.is_game_over()){
			System.out.println("              Round " + round );
			int currentPlayer = bd.currentPlayer;
			System.out.println("Player " + currentPlayer + "\'s move.");
			int bin = bd.move();
			if (bin <= 0) break;
			System.out.println("Player " + currentPlayer + " selects "
		                         + bd.stonesMoved + " stones from bin " + bin );
			System.out.println(bd);
			round++;
		}
		System.out.println( "Final board configuration:\n" );

		System.out.println( bd );
		if (bd.get_bin( 0 , 0 ) == bd.get_bin( 1 , 0 )) {
			System.out.println( "It's a tie!");
			return -1;
		} else if (bd.get_bin( 0 , 0 ) > bd.get_bin( 1 , 0 )) {
			System.out.println( "Player0 wins!" );
			return 0;
		} else {
			System.out.println( "Player1 wins!" );
			return 1;
		}
	}


	private static int play_human(Heuristic s0 , int MAX_DEPTH ) {
		//Heuristic s1 = new Heuristic(1);
		board bd = new board(s0 , null , MAX_DEPTH );
		
        System.out.println(bd);
		int round = 0;
		int bin;
		int currentPlayer;
		boolean turnAI;
		Scanner sc = new Scanner(System.in);
		System.out.println("First Move by AI(1) or by you(2)?");
		int ch = sc.nextInt();
		if(ch==1){
			turnAI = true;
			currentPlayer = 0;
			bd.currentPlayer = 0;
		}else{
			turnAI = false;
			currentPlayer = 1;
			bd.currentPlayer = 1;
		}
		while(!bd.is_game_over()){
			System.out.println("              Round " + round );
			currentPlayer = bd.currentPlayer;
			if(currentPlayer == 0)System.out.println("AI\'s move\n");
			else System.out.println("Player " + currentPlayer + "\'s move.");
			
			if(turnAI && currentPlayer==0){
				bin = bd.move();
				if(bd.currentPlayer == 1)turnAI = false;
			}
			else {
				System.out.println("Select bin[1/2/3/4/5/6](right to left)\n");
				bin = sc.nextInt();
				bd.move(bin);
				if(bd.currentPlayer == 0)turnAI = true;
			}
			
			if (bin <= 0) break;
			if(currentPlayer == 0)System.out.println("AI selects " + bd.stonesMoved + " stones from bin " + bin);
			else System.out.println("Player " + currentPlayer + " selects " + bd.stonesMoved + " stones from bin " + bin );
			
			System.out.println(bd);
			round++;
		}
		System.out.println( "Final board configuration:\n" );

		System.out.println( bd );
		if (bd.get_bin( 0 , 0 ) == bd.get_bin( 1 , 0 )) {
			System.out.println( "It's a tie!" );
			return -1;
		} else if (bd.get_bin( 0 , 0 ) > bd.get_bin( 1 , 0 )) {
			if(human_vs_AI)System.out.println( "AI wins!" );
			else System.out.println( "Player0 wins!" );
			return 0;
		} else {
			System.out.println( "Player1 wins!" );
			return 1;
		}
	}

	public static void stat()throws IOException {
		PrintWriter logFile = new PrintWriter(new FileWriter("log.log") );
		int no_of_heuristics = h_list0.length;
		ReportObject [][]data = new ReportObject[no_of_heuristics+1][no_of_heuristics+1];
		for(int i=1 ; i<=no_of_heuristics ; i++){
			for(int j=1 ; j<=no_of_heuristics ; j++){
				data[i][j] = new ReportObject();
			}
		}

		int h0, h1;
		for(int d=5 ; d<=13 ; d++){
			logFile.println("Depth "+d);
			for (int i : h_list0){
				for (int j : h_list1){
					if(i==j)continue;
				
					h0 = i;
					h1 = j;
					
					int n0 = 0, n1 = 0, tie = 0;
					//i first move,j 2nd move
					for (int game = 0; game <NO_GAMES_PER_HEURISTIC ; ++game) {
						int r = play(selectStrategy( h0 ) , selectStrategy( h1 ) , d );
						if (r == 0) n0++;
						if (r == 1) n1++;
						if (r == -1) tie++;
					}
					
					logFile.println("\n\nHeuristic "+h0+" vs "+h1+"\nFirst Move : "+h0);
					logFile.println( "Heuristic " + h0 + " won : " + n0 + "\nHeuristic "
									+ h1 + " won : " + n1 + "\nDraw : " + tie +"\n");
					logFile.flush();
					data[i][j].winPercentage_firstMove = ((double)n0/NO_GAMES_PER_HEURISTIC)*100.0;
					data[j][i].winPercentage_secondMove = ((double)n1/NO_GAMES_PER_HEURISTIC)*100.0;   										
				}
			}
			PrintWriter out=null;
			try {
				out = new PrintWriter("depth_"+d+".csv");
			} catch (Exception e) {
				//TODO: handle exception
			}
			StringBuilder sb = new StringBuilder();
			sb.append(" ,h1,h2,h3,h4,h5,h6\n");
			//sb.append(" ,h1,h2\n");
			for(int p=1 ; p<=no_of_heuristics ; p++){
				sb.append("h"+p+",");
				for(int q=1 ; q<=no_of_heuristics ; q++){
					if(p==q)sb.append("-");
					else{
						sb.append("win%1st "+data[p][q].winPercentage_firstMove
								+" win%2nd "+data[p][q].winPercentage_secondMove);
					} 
					if(q==no_of_heuristics)sb.append(",\n");
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
		//generate report
		if(generate_report){
			stat();
			return;
		}
		int h[] = { 0 , 0 };
		Scanner scanner = new Scanner( System.in );
		System.out.println("AI vs AI(1)\nor\nHuman vs AI(2)\n");
		int choice = scanner.nextInt();
		if (choice==2){
			//human vs AI
			human_vs_AI = true;
			System.out.println("Choose heuristic[1/2/3/4/5/6] for AI(player 0)");
			int h_id = scanner.nextInt();
			Heuristic s0 = new Heuristic(h_id);
			play_human(s0, 10);
		} 
		else{	
            System.out.println("Choose heuristic[1/2/3/4/5/6] for player 0");
            h[0] = scanner.nextInt();
            System.out.println("Choose heuristic[1/2/3/4/5/6] for player 1");
            h[1] = scanner.nextInt();
			System.out.println("Choose depth");
            int depth = scanner.nextInt();
            play(selectStrategy(h[0]) , selectStrategy(h[1]) , depth);
		}
	}

}

class ReportObject{
	double winPercentage_firstMove;
	double winPercentage_secondMove;

	ReportObject(){}
}