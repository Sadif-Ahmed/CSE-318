

public class heuristics{

    public int hamming_distance(Board b){
        int cnt = 0;
        for(int i=1; i<=b.size ; i++){
            for(int j=1 ; j<=b.size ; j++){
                if(b.matrix[i][j]==0)continue;

                Pair p = get_correct_position(b.matrix[i][j] , b);

                if(p.get_x()!= i || p.get_y()!=j)cnt++;
            }
        }
        return cnt;
    }

    public int manhattan_distance(Board b){
        int cnt = 0;
        for(int i=1; i<=b.size ; i++){
            for(int j=1 ; j<=b.size ; j++){
                if(b.matrix[i][j]==0)continue;

                Pair p = get_correct_position(b.matrix[i][j] , b);

                cnt=cnt+(Math.abs(p.get_x()-i)+Math.abs(p.get_y()-j));
            }
        }
        return cnt;
    }

    public int linear_conflicts(Board b) {
        int md = manhattan_distance(b);
        int cnt = 0;

        for(int i=1 ; i<=b.size ; i++){
            for(int j=1 ; j<=b.size ; j++){
                if(b.matrix[i][j]==0)continue;

                Pair p = get_correct_position(b.matrix[i][j] , b);

                if(p.get_x()==i){
                    for(int k = j+1 ; k<=b.size ; k++){
                        if(b.matrix[i][k]==0)continue;

                        Pair pk = get_correct_position(b.matrix[i][k] , b);

                        if(pk.get_x()==p.get_x() && pk.get_y()<p.get_y()){
                            cnt++;
                        }
                    }
                }
                if(p.get_y()==j){
                    for(int k = i+1 ; k<=b.size ; k++){
                        if(b.matrix[k][j]==0)continue;

                        Pair pk = get_correct_position(b.matrix[k][j] , b);

                        if(pk.get_x() < p.get_x() && pk.get_y() == p.get_y()){
                            cnt++;
                        }
                    }
                }
            }
        }
        return md+2*cnt;
    }


    public Pair get_correct_position(int val , Board b){
        Integer x,y;
        if (val % b.size != 0) {
            x = 1 + val / b.size;
            y = val % b.size;
        } else {
            x = val / b.size;
            y = b.size;
        }
        return new Pair(x, y);
        
    }

} 