import java.util.Comparator;

public class Node_Comp implements Comparator<Search_Node>{
    @Override
    public int compare(Search_Node x,Search_Node y)
    {
        if(x.f_n==y.f_n)
        {
            if(x.h_n>y.h_n)
            {
                return 1;
            }
            else if(x.h_n<y.h_n)
            {
                return -1;
            }
            else 
            {
                return 0;
            }
        }
        else if(x.f_n>y.f_n)
        {
            return 1;
        }
        else if(x.f_n<y.f_n)
        {
            return -1;
        }
        else
        {
            return 0;
        }

    }
    
}
