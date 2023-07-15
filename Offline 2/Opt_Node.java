public class Opt_Node {
    Tree_Node node;
    double h_value;

    Opt_Node(Tree_Node node,double h_value)
    {
        this.node=node;
        this.h_value=h_value;
    }
    public static Opt_Node max(Opt_Node opt1,Opt_Node opt2)
    {
        if(opt2==null) return opt1;
        else if(opt1!=null && opt1.h_value >= opt2.h_value) return opt1;
        else return opt2;
    }
    public static Opt_Node min(Opt_Node opt1,Opt_Node opt2)
    {
        if(opt2==null) return opt1;
        else if(opt1!=null && opt1.h_value <= opt2.h_value) return opt1;
        else return opt2;
    }
}
