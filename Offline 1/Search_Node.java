public class Search_Node {
    Board board;
    int g_n;
    int h_n;
    int f_n;
    Search_Node parent;
    
    Search_Node(Board board,int g_n,int h_n)
    { 
        this.board=board;
        this.g_n=g_n;
        this.h_n=h_n;
        this.f_n=g_n+h_n;
    }
}
