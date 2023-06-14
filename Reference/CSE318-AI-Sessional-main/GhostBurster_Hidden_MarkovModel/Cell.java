import java.util.*;
import java.lang.*;
import java.io.*;
import java.math.BigDecimal;
import java.math.RoundingMode;
import java.util.Objects;

public class Cell {
    private int x,y;
    private double ghostProbability;
    private boolean isBlocked;
    private double side_move_prob;
    private double non_side_move_prob;

    public Cell(int x, int y) {
        this.x = x;
        this.y = y;
        this.isBlocked = false;
    }

    public void set_side_move_prob(double p){
        side_move_prob = p;
    }

    public void set_non_side_move_prob(double p){
        non_side_move_prob = p;
    }

    public double get_side_move_prob(){
        return side_move_prob;
    }

    public double get_non_side_move_prob(){
        return non_side_move_prob;
    }

    



    public boolean isBlockedCell(){
        return isBlocked;
    }

    public void setBlocked(){
        this.isBlocked = true;
    }

    public void initializeProbability(double ghostProbability){
        this.setGhostProbability(ghostProbability);
    }

    public int calcManhattanDistance(Cell cell){
        int delCol = Math.abs(this.getX() - cell.getX());
        int delRow = Math.abs(this.getY() - cell.getY());
        int distance = delRow + delCol;
        return distance;
    }

    public double getGhostProbability() {
        return ghostProbability;
    }

    public void setGhostProbability(double ghostProbability) {
        BigDecimal bd = new BigDecimal(ghostProbability).setScale(5, RoundingMode.HALF_UP);
        double val2 = bd.doubleValue(); 
        this.ghostProbability = val2;
    }

    public int getX() {
        return x;
    }

    public void setX(int x) {
        this.x = x;
    }

    public int getY() {
        return y;
    }

    public void setY(int y) {
        this.y = y;
    }

    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (o == null || getClass() != o.getClass()) return false;
        Cell cell = (Cell) o;
        return x == cell.x &&
                y == cell.y;
    }

    @Override
    public int hashCode() {
        return Objects.hash(x, y);
    }

    @Override
    public String toString() {
        return "Cell(" +
                 x +
                " , " + y +
                ")";
    }
}
