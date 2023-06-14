public class ConsolePrint{
    private final int bins = 6;
    private int ara[][];
    private final String RESET = "\u001B[0m";
	private final String GREEN = "\u001B[32m";
	private final String YELLOW = "\u001B[33m";
	private final String BLUE = "\u001B[34m";
	private final String PURPLE = "\u001B[35m";

    public ConsolePrint(int [][]ara){
        this.ara = ara;
    }

    public String edgeLine() {
		return YELLOW + "+----" + middleDashes() + "----+\n" + RESET;
	}
	
	public String player0Line() {
		StringBuilder sb = new StringBuilder();
		sb.append(YELLOW+ "|    |" +RESET);
		for (int i = 1; i <= bins; i++){
			sb.append( " " ).append(GREEN + numberString(ara[0][i]) + RESET ).append(YELLOW+ " |" + RESET);
		}
		sb.append(YELLOW + "    |\n" +RESET);
		return sb.toString();
	}
	
	public String middleLine() {
		return YELLOW+"| " +RESET+ PURPLE + numberString(ara[0][0]) + RESET + " "
				       + YELLOW+ middleDashes()+RESET
				       + " " + PURPLE + numberString(ara[1][0]) + RESET + YELLOW+" |\n"+RESET;
	}
	
	public String player1Line() {
		StringBuilder sb = new StringBuilder();
		sb.append(YELLOW+ "|    |" +RESET);
		for (int i = bins; i > 0; i--) {
			sb.append( " " ).append(BLUE + numberString(ara[1][i]) + RESET ).append( YELLOW+ " |" + RESET);
		}
		sb.append( YELLOW + "    |\n" +RESET);
		return sb.toString();
	}
	
	public String middleDashes() {
		StringBuilder sb = new StringBuilder();
		for (int i = 1; i <= bins; i++) {
			sb.append( "+----" );
		}
		sb.append( "+" );
		return sb.toString();
	}
	
	public String numberString( int n ) {
		if ((0 <= n) && (n < 10)) {
			return " " + n;
		} else {
			return Integer.toString( n );
		}
	}

}