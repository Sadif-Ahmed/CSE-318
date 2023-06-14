
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.lang.Math.*;
import java.util.Scanner;
import java.math.BigDecimal;
import java.math.RoundingMode;

public class Game {
    private static int ROW;
    private static int COL;
    public static Cell[][] board;
    private static double tempProb [][];
    private static double p =0.9;//side move prob

    public Game(int ROW, int COL) {
        this.ROW = ROW;
        this.COL = COL;
        board = new Cell[ROW][COL];
        tempProb = new double[ROW][COL];
        double inProb = 1.0/(ROW*COL);
        for(int i = 0 ; i < ROW ; i++){
            for(int j = 0 ; j < COL ; j++) {
                board[i][j] = new Cell(j, i);
                board[i][j].initializeProbability(inProb);
            }
        }
    }

    public Cell[][] getBoard() {
        return board;
    }

    //if cell2 is adj to cell1
    //adj means among those 9 cells
    private static boolean is_adj(Cell cell1 , Cell cell2){
        if(Math.abs(cell1.getY() - cell2.getY())<=1 && Math.abs(cell1.getX() - cell2.getX())<=1){
            return true;
        }
        else return false;
    }

    private static void updateSensorbasedProbability(Cell cell , int ghost_present){
        double prob;
        double sensor_prob = 0.85;
        for(int i=0; i<ROW ; i++){
            for(int j=0 ; j<COL; j++){
                if(ghost_present == 1){//sensor reading 1
                    if(is_adj(cell, board[i][j])){
                        prob = board[i][j].getGhostProbability() * sensor_prob;
                    }
                    else{
                        prob = board[i][j].getGhostProbability() * (1-sensor_prob); 
                    }
                }
                else{//sensor reading 0
                    if(is_adj(cell, board[i][j])){
                        prob = board[i][j].getGhostProbability() * (1-sensor_prob);
                    }
                    else{
                        prob = board[i][j].getGhostProbability() * sensor_prob; 
                    }
                }
                tempProb[i][j] = prob; 
                
            }
        }       
    }

    public static void updateObservation(Cell cell , int ghost_present){
        updateSensorbasedProbability(cell, ghost_present);
        normalizeProbability();
    }

    public static void normalizeProbability(){
        double totalProbs = 0.0;
        for(int i = 0 ; i < ROW ; i++){
            for(int j = 0 ; j < COL ; j++) {
                totalProbs += tempProb[i][j];
            }
        }
        double prob;
        for(int i = 0 ; i < ROW ; i++){
            for(int j = 0 ; j < COL ; j++) {
                prob = tempProb[i][j];
                board[i][j].setGhostProbability(prob/totalProbs);
            }
        }
    }

    public static void set_valid_moves(Cell cell){
        int valid_side_moves = 0;
        int valid_non_side_move = 0;

        int i1 = (cell.getX() == 0)?cell.getX():cell.getX()-1;
        int i2 = (cell.getX() == ROW-1)?cell.getX():cell.getX()+1;
        int j1 = (cell.getY() == 0)?cell.getY():cell.getY()-1;
        int j2 = (cell.getY() == COL-1)?cell.getY():cell.getY()+1;
        
        for(int i = i1 ; i <= i2 ; i++){
            for(int j = j1 ; j <= j2 ; j++){
                //System.out.printf("(%d %d) --- " , i ,j);
                if(board[i][j].isBlockedCell()){
                    continue;
                }
                else if(i == cell.getX() && j == cell.getY()){    
                    valid_non_side_move++;//self
                }
                else if(i != cell.getX() && j != cell.getY()){
                    //diag
                    valid_non_side_move++;
                }
                else if(i == cell.getX() || j == cell.getY()){
                    //sideways
                    valid_side_moves++;
                }
            }
        }
        p = 0.9;
        if(valid_side_moves!=0)cell.set_side_move_prob(p/valid_side_moves);
        else{
            cell.set_side_move_prob(0.0);
            p = 0.0;
        } 
        if(valid_non_side_move!=0)cell.set_non_side_move_prob((1-p)/(valid_non_side_move));
        else{
            cell.set_non_side_move_prob(0.0);
        } 
    }

    public static double move_prob(Cell cell1 , Cell cell2){
        if(cell2.isBlockedCell())return 0.0;

        if(cell1.getX()==cell2.getX() && cell1.getY() != cell2.getY()){
            return cell1.get_side_move_prob();    
        }
        else if(cell1.getX() != cell2.getX() && cell1.getY() == cell2.getY()){
            return cell1.get_side_move_prob();   
        }
        else if(cell2.getX()==cell1.getX()-1 && Math.abs(cell1.getY()-cell2.getY())==1){
            return cell1.get_non_side_move_prob();
        }
        else if(cell2.getX()==cell1.getX()+1 && Math.abs(cell1.getY()-cell2.getY())==1){
            return cell1.get_non_side_move_prob();
        }
        else if(cell1.getX()==cell2.getX() && cell1.getY()==cell2.getY()){
            return cell1.get_non_side_move_prob();
        }
        else {
            return 0.0;
        }
    }

    private static double updateProbability(Cell cell){
        int i1 = (cell.getX() == 0)?cell.getX():cell.getX()-1;
        int i2 = (cell.getX() == ROW-1)?cell.getX():cell.getX()+1;
        int j1 = (cell.getY() == 0)?cell.getY():cell.getY()-1;
        int j2 = (cell.getY() == COL-1)?cell.getY():cell.getY()+1;
        
        double sumOfProbs = 0.0;
        double moveProb=0.0;

        for(int i = i1 ; i <= i2 ; i++){
            for(int j = j1 ; j <= j2 ; j++){
                moveProb = move_prob(board[i][j] , cell) * board[i][j].getGhostProbability();
                sumOfProbs += moveProb;
            }
        }
        
        return sumOfProbs;
    }


    //set prob of side/diag move
    public static void set_prob_side_diag_self_move(){
        for(int i = 0 ; i < ROW ; i++){
            for(int j = 0 ; j < COL ; j++) {
                set_valid_moves(board[i][j]);
            }
        }
    }

    public static void updateTransition(){
        for(int i = 0 ; i < ROW ; i++){
            for(int j = 0 ; j < COL ; j++) {
                tempProb[i][j] = updateProbability(board[i][j]);
            }
        }
        for(int i = 0 ; i < ROW ; i++){
            for(int j = 0 ; j < COL ; j++) {
                board[i][j].setGhostProbability(tempProb[i][j]);
            }
        }
    }

    

    private static double get_sum(){
        double sum = 0.0;
        for(int i = 0 ; i < ROW ; i++){
            for(int j = 0 ; j < COL ; j++) {
                sum = sum + board[i][j].getGhostProbability();
            }
        }
        return sum;
    }

    public  static void printBoard(){
        for(int i = 0 ; i < ROW ; i++){
            for(int j = 0 ; j < COL ; j++) {
                System.out.printf("%.3f\t",board[i][j].getGhostProbability());
            }
            System.out.println();
            System.out.println();
        }
        System.out.printf("Sum = %.3f\n\n" , get_sum());
    }

    public static void printTempBoard(){
        System.out.println("TEMPPPPPPP");
        for(int i = 0 ; i < ROW ; i++){
            for(int j = 0 ; j < COL ; j++) {
                System.out.printf("%.3f\t",tempProb[i][j]);
            }
            System.out.println();
            System.out.println();
        }
        System.out.println();
    }

    public static void print_max_prob_position(){
        double prob = -1.0;
        Cell cell=new Cell(-1,-1);
        for(int i = 0 ; i < ROW ; i++){
            for(int j = 0 ; j < COL ; j++) {
                if(board[i][j].getGhostProbability() > prob){
                    prob = board[i][j].getGhostProbability();
                    cell = board[i][j];
                }
            }
        }
        System.out.println("Casper is most likely in "+ cell.toString());
    }

      
public static void main(String[] args) throws FileNotFoundException {
    //Scanner input = new Scanner(System.in);
    Scanner in = new Scanner(new FileReader("input.txt"));
    ROW = in.nextInt();
    COL = in.nextInt();
    int no_of_obstacles = in.nextInt();
    board = new Cell[ROW][COL];
    tempProb = new double[ROW][COL];
    
    double inProb = 1.0/(ROW*COL-no_of_obstacles);
    for(int i = 0 ; i < ROW ; i++){
        for(int j = 0 ; j < COL ; j++) {
            board[i][j] = new Cell(i, j);
            board[i][j].initializeProbability(inProb);
        }
    }
    
    int r_obs , c_obs;
    for(int i=0;i<no_of_obstacles;i++){
        r_obs = in.nextInt();
        c_obs = in.nextInt();
        board[r_obs][c_obs].setBlocked();
        board[r_obs][c_obs].initializeProbability(0.0);
    }

    set_prob_side_diag_self_move();

    System.out.println("Initial Probabilities\n");
    printBoard();

    String s;int p,q,res;
    while(true){
        s = in.nextLine();
        if(s.contains("R")){
            System.out.println(s);
            p = s.charAt(2)-'0';
            q = s.charAt(4)-'0';
            res = s.charAt(6)-'0';
            updateTransition();
            System.out.println("Updated Grid");
            updateObservation(board[p][q], res);
            printBoard();
        }
        else if(s.contains("C")){
            //get max prob
            print_max_prob_position();
        }
        else if(s.contains("Q")){
            System.out.println("Bye Casper\n");
            break;    
        }
        
    }

}


}

