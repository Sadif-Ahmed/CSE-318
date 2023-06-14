public class OptimalNode {
		GameTreeNode node;
		double heuristic_value;
		
		OptimalNode( GameTreeNode node , double heuristic_value ) {
			this.node = node;
			this.heuristic_value = heuristic_value;
		}
		
		public static OptimalNode max( OptimalNode opt1 , OptimalNode opt2 ){
            if (opt2 == null)return opt1;
			else if(opt1 != null && opt1.heuristic_value >= opt2.heuristic_value) return opt1;
			else return opt2;
		}
		
		public static OptimalNode min( OptimalNode opt1 , OptimalNode opt2){
            if (opt2 == null)return opt1;
			else if(opt1 != null && opt1.heuristic_value <= opt2.heuristic_value) return opt1;
			else return opt2;
		}

	}
	