import java.util.ArrayList;

public interface GameTreeNode {
	boolean isTerminal();
	boolean isMaximizing();
	double heuristicValue();
	ArrayList< GameTreeNode > successors();
	boolean equals( GameTreeNode o );
}
