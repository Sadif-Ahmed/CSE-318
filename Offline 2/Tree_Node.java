import java.util.*;

public interface Tree_Node {
    boolean terminal_check();
    boolean maximizing_check();
    double heuristic_value();
    ArrayList < Tree_Node > get_successor_list();
    boolean equals(Tree_Node o);
}
