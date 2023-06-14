import java.util.Comparator;

/**
 *
 * @author Afnan
 */

public class NodeComparator implements Comparator<Node>{
    @Override
    public int compare(Node a , Node b){
        if(a.f_cost==b.f_cost){
            if(a.h_cost < b.h_cost){
                return -1;
            }
            else if(a.h_cost>b.h_cost){
                return 1;
            }
            else return 0;
        }
        else if(a.f_cost < b.f_cost){
            return -1;
        }
        else if(a.f_cost > b.f_cost){
            return 1;
        }
        else return 0;
    }
}