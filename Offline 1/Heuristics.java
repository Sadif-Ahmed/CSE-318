
public class Heuristics {
    
    public Cell get_corrct_pos(int value,int board_size)
    {
        int x,y;
        if(value % board_size!=0)
        {
            x= 1+value/board_size;
            y= value%board_size;
        }
        else
        {
            x = value/board_size;
            y= board_size; 
        }
        return new Cell(x, y);
    }
    public int hamming_distance(Board board)
    {
        int ham_dist=0;
        for(int i=1;i<=board.size;i++)
        {
            for(int j=1;j<=board.size;j++)
            {
                if(board.mat[i][j]==0) continue;
                Cell temp = get_corrct_pos(board.mat[i][j], board.size);
                if(i!= temp.get_x()|| j!= temp.get_y())
                {
                    ham_dist++;
                }
            }
        }
        return ham_dist;
    }
    public int manhattan_distance(Board board)
    {
        int man_dist=0;
        for(int i=1;i<=board.size;i++)
        {
            for(int j=1;j<=board.size;j++)
            {
                if(board.mat[i][j]==0) continue;
                Cell temp = get_corrct_pos(board.mat[i][j], board.size);
                man_dist += (Math.abs(temp.get_x()-i)+Math.abs(temp.get_y()-j));
            }
        }
        return man_dist;
    }
}
