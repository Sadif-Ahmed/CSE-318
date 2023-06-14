/**
 *
 * @author Afnan
 */

public class Node {
    Board board;
    int g_cost;
    int h_cost;
    int f_cost;
    Node parent;

    public Node(Board board, int g_cost,int h_cost) {
        this.board = board;
        this.g_cost = g_cost;
        this.h_cost=h_cost;
        this.f_cost=this.g_cost+this.h_cost;
    }
}