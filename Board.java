import java.util.Arrays;

public class Board {
    int mat [][];
    int size;
    public Board(int [][] mat,int size)
    {
        this.size=size;
        this.mat =  new int[size+1][size+1];
        for(int i=1;i<=size;i++)
        {
            for(int j=1;j<=size;j++)
            {
                this.mat[i][j]=mat[i][j];
            }
        }
    }
    public static Board get_goal(int size)
    {
        int [][]final_board = new int[size+1][size+1];
        int temp=1;
        for(int i=1;i<=size;i++)
        {
            for(int j=1;j<=size;j++)
            {
                final_board[i][j]=temp++;
            }
        }
        final_board[size][size]=0;
        return new Board(final_board, size);
    }
    public Board swap_cell(int x,int y,int p,int q) {
        int [][] temp_board = new int [this.size+1][this.size+1];
        int temp;
        for(int i=1;i<=size;i++)
        {
            for(int j=1;j<=size;j++)
            {
                temp_board[i][j]=this.mat[i][j];
            }
        }
        temp=temp_board[x][y];
        temp_board[x][y]=temp_board[p][q];
        temp_board[p][q]=temp;
        return new Board(temp_board, this.size);
    }
    public int invertion_count()
    {
        int invertion_count=0;
        int []temp =  new int[size*size];
        int idx=0;
        for(int i=1;i<=size;i++)
        {
            for(int j=1;j<=size;j++)
            {
                temp[idx++]=this.mat[i][j];
            }
        }
        for(int i=0;i<size*size-1;i++)
        {
            for(int j=i+1;j<size*size;j++)
            {
                if(temp[j]<temp[i])
                {
                    invertion_count++;
                }
            }
        }
        return invertion_count;
    }
    @Override
    public boolean equals(Object obj)
    {
        if(this == obj)
        {
            return true;
        }
        if(obj==null)
        {
            return false;
        }
        if(getClass() != obj.getClass())
        {
            return false;
        }
        final Board temp = (Board) obj;
        for(int i=1;i<=this.size;i++)
        {
            for(int j=1;j<=this.size;j++)
            {
                if(temp.mat[i][j]!=this.mat[i][j])
                {
                    return false;
                }
            }
        }
        return true;
        
    }
    public void print() {
        for(int i=1;i<=size;i++)
        {
            for(int j=1;j<=size;j++)
            {
                if(mat[i][j]==0)
                {
                System.out.println("* ");
                }
                else
                {
                    System.out.println(mat[i][j]+" ");
                }
            }
            System.out.println("");
        }
    }
    @Override
    public int hashCode()
    {
        int val= Arrays.deepHashCode(this.mat);
        return Math.abs(val);
    }
}
