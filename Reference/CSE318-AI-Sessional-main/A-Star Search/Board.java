import java.util.Arrays;

/**
 *
 * @author Afnan
 */

public class Board{

    int matrix[][];
    int size;

    public Board(int[][] matrix , int n){
        this.size = n;
        this.matrix = new int[n+1][n+1];
        for(int i=1 ; i<=n ; i++){
            this.matrix[i] = Arrays.copyOf(matrix[i], matrix[i].length);
        }
    }

    public static Board get_final_state(int n){
        int val = 1;
        int [][]data = new int[n+1][n+1];
        for(int i=1 ; i<=n ; i++){
            for(int j=1 ; j<=n ; j++){
                data[i][j] = val++;
            }
        }
        data[n][n] = 0;
        return new Board(data , n);
    }

    public Board move_blank(int x , int y , int p , int q){
        int [][]data = new int[size+1][size+1];
        for(int i=1 ; i<=size; i++){
            for(int j=1 ; j<=size ; j++){
                data[i] = Arrays.copyOf(matrix[i], matrix[i].length);
            }
        }
        int tmp = data[p][q];
        data[p][q] = data[x][y];
        data[x][y] = tmp;

        return new Board(data , size);
    }

    public int get_inversion_count(){
        int []ara = new int[size*size + 2];
        int idx=0;
        for(int i=1 ; i<=size; i++){
            for(int j=1 ; j<=size ; j++){
                ara[idx++] = matrix[i][j];
            }
        }

        int inv = 0;
        int ara_sz = size*size;
        for(int i=0 ; i<ara_sz-1 ; i++){
            for(int j=i+1 ; j<ara_sz ; j++){
                if(ara[i]==0 || ara[j]==0)continue;
                if(ara[i] > ara[j]){
                    inv++;
                }
            }
        }
        return inv;

    }

    public void printBoard(){
        for(int i=1 ; i<=size ; i++){
            for(int j=1 ;j<=size ; j++){
                if(matrix[i][j]==0)
                {
                    
                    System.out.print("*  ");
                }
                else
                {
                    System.out.print(matrix[i][j]+"  ");
                }
            }
            System.out.println("");
        }
    }

    @Override
    public boolean equals(Object obj) {

        if (this == obj) {
            return true;
        }
        if (obj == null) {
            return false;
        }
        if (getClass() != obj.getClass()) {
            return false;
        }
        final Board other = (Board)obj;
        for(int i=1;i<=this.size;i++)
        {
            for(int j=1;j<=this.size;j++)
            {
                if( other.matrix[i][j] != this.matrix[i][j] )
                {
                    return false;
                }
            }
        }
        return true;
    }

    @Override
    public int hashCode()
    {
        int val=Arrays.deepHashCode(this.matrix);
        return Math.abs(val);
    }


}
