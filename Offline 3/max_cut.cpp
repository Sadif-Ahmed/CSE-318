#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <iterator>
#include <climits>
#include<bits/stdc++.h>
#include<cmath>
#include<limits.h>
#include <fstream>
#include <sstream>
using namespace std;
typedef pair<long int,double> dest_weight;
typedef pair<long int,long int> vertice;
typedef pair<vertice,double> edge;
typedef pair<pair<set<long int>,set<long int>>,double> cut;
#define inf 9999999.9999999
class Graph
{
    public:
    long int num_of_vertices;
    long int num_of_edges;
    vector<dest_weight> *dir_graph;
    vector<dest_weight> *rev_dir_graph;
    vector<edge> List_of_Edges;//Storing the edges
    double **Adj_Matt;
    //maxcut
    set<long int> all_veritces;

    Graph(long int num_v,long int num_e)
    {
        num_of_vertices=num_v;
        num_of_edges=num_e;
        dir_graph= new vector<dest_weight>[num_of_vertices];
        rev_dir_graph= new vector<dest_weight>[num_of_vertices];
        List_of_Edges.clear();
        Adj_Matt=new double*[num_of_vertices];
        for(long int i=0; i<num_of_vertices; i++)
        {
            Adj_Matt[i]=new double[num_of_vertices];
        }
        for(long int i=0; i<num_of_vertices; i++)
        {
            for(long int j=0; j<num_of_vertices; j++)
            {
                if(i==j)
                {
                    Adj_Matt[i][j]=0;
                }
                else
                {
                    Adj_Matt[i][j]=inf ;
                }
            }
        }
        for(long int i=0;i<num_of_vertices;i++)
        {
            all_veritces.insert(i);
        }

    }
    vector<pair<vertice,double>> get_graph_edges()
    {
        return List_of_Edges;
    }
    void Add_Edge_Directed(long int startv,long int endv,double weight)
    {
        dir_graph[startv].push_back(make_pair(endv,weight));
        rev_dir_graph[endv].push_back(make_pair(startv,weight));
        List_of_Edges.push_back(make_pair(make_pair(startv,endv),weight));
        Adj_Matt[startv][endv]=weight;
        
    }
    void Add_Edge_Undirected(long int startv,long int endv,double weight)
    {
        dir_graph[startv].push_back(make_pair(endv,weight));
        dir_graph[endv].push_back(make_pair(startv,weight));
        List_of_Edges.push_back(make_pair(make_pair(startv,endv),weight));
        List_of_Edges.push_back(make_pair(make_pair(endv,startv),weight));
        Adj_Matt[startv][endv]=weight;
        Adj_Matt[endv][startv]=weight;
        
    }
    void Remove_Edge_Directed(long int startv,long int endv,double weight)
    {

        for(long int i=0; i<List_of_Edges.size(); i++)
        {
            if(List_of_Edges[i].first.first==startv&&List_of_Edges[i].first.second==endv&&List_of_Edges[i].second==weight)
            {
                List_of_Edges.erase(List_of_Edges.begin()+i);
            }
        }
        for(long int j=0; j<dir_graph[startv].size(); j++)
        {
            if(dir_graph[startv][j].first==endv&&dir_graph[startv][j].second==weight)
            {
                dir_graph[startv].erase(dir_graph[startv].begin()+j);
            }
        }
        for(long int j=0; j<rev_dir_graph[endv].size(); j++)
        {
            if(rev_dir_graph[endv][j].first==startv&&rev_dir_graph[endv][j].second==weight)
            {
                rev_dir_graph[endv].erase(rev_dir_graph[endv].begin()+j);
            }
        }
        Adj_Matt[startv][endv]=inf ;
        

    }
    void Remove_Edge_UnDirected(long int startv,long int endv,double weight)
    {

        for(long int i=0; i<List_of_Edges.size(); i++)
        {
            if(List_of_Edges[i].first.first==startv&&List_of_Edges[i].first.second==endv&&List_of_Edges[i].second==weight)
            {
                List_of_Edges.erase(List_of_Edges.begin()+i);
            }
            if(List_of_Edges[i].first.first==startv&&List_of_Edges[i].first.second==endv&&List_of_Edges[i].second==weight)
            {
                List_of_Edges.erase(List_of_Edges.begin()+i);
            }
        }
        for(long int j=0; j<dir_graph[startv].size(); j++)
        {
            if(dir_graph[startv][j].first==endv&&dir_graph[startv][j].second==weight)
            {
                dir_graph[startv].erase(dir_graph[startv].begin()+j);
            }
        }
        for(long int j=0; j<dir_graph[endv].size(); j++)
        {
            if(dir_graph[endv][j].first==startv&&dir_graph[endv][j].second==weight)
            {
                dir_graph[endv].erase(dir_graph[endv].begin()+j);
            }
        }
        Adj_Matt[startv][endv]=inf ;
        Adj_Matt[endv][startv]=inf ;
        

    }
    void print_AdjM()
    {
        cout<<"Adjacency Matrix"<<endl;
        for(long int i=0; i<num_of_vertices; i++)
        {
            for(long int j=0; j<num_of_vertices; j++)
            {
                if(Adj_Matt[i][j]==inf )
                {
                    cout<<"INF  ";
                }
                else
                {
                    cout<<Adj_Matt[i][j]<<"  ";
                }
            }
            cout<<endl;
        }
    }
    void sort_by_edges()//Order of E(square)
    {
        for(long int i=0; i<num_of_edges; i++)
        {
            for(long int j=i+1; j<num_of_edges; j++)
            {
                if(List_of_Edges[i].second>=List_of_Edges[j].second)
                {
                    pair<vertice,double> temp;
                    temp=List_of_Edges[i];
                    List_of_Edges[i]=List_of_Edges[j];
                    List_of_Edges[j]=temp;
                }
            }
        }
    }
    void sort_by_edges_r()//Order of E(square)
    {
        for(long int i=0; i<num_of_edges; i++)
        {
            for(long int j=i+1; j<num_of_edges; j++)
            {
                if(List_of_Edges[i].second<=List_of_Edges[j].second)
                {
                    pair<vertice,double> temp;
                    temp=List_of_Edges[i];
                    List_of_Edges[i]=List_of_Edges[j];
                    List_of_Edges[j]=temp;
                }
            }
        }
    }
    
    void Print_Edges(vector<pair<vertice,double>> test)//Order of V
{

    cout<<"Edges"<<"   "<<"Weight"<<endl;
    for(long int i=0; i<test.size(); i++)
    {
        cout<<test[i].first.first<<" -- "<<test[i].first.second<<"  "<<test[i].second<<endl;
    }
}
edge heaviest_edge()
{
    long int u,v;
    double max=0;
    for(long int i=0;i<num_of_vertices;i++)
    {
        for (long int j=0;j<num_of_vertices;j++)
        {
            if(Adj_Matt[i][j]!=inf  && Adj_Matt[i][j]!=0 && Adj_Matt[i][j]>max)
            {
                max=Adj_Matt[i][j];
                u=i;
                v=j;
            }
        }
    }
    return make_pair(make_pair(u,v),max);
}
edge lightest_edge()
{
    long int u,v;
    double max=inf ;
    for(long int i=0;i<num_of_vertices;i++)
    {
        for (long int j=0;j<num_of_vertices;j++)
        {
            if(Adj_Matt[i][j]!=inf  && Adj_Matt[i][j]!=0 && Adj_Matt[i][j]<max)
            {
                max=Adj_Matt[i][j];
                u=i;
                v=j;
            }
        }
    }
    return make_pair(make_pair(u,v),max);
}
cut semi_greedy_maxcut()
{
    cut final;
    double final_weight=0;
    double lower_bound = 0;
    double upper_bound = 1;
    uniform_real_distribution<double> unif(lower_bound,upper_bound);
    default_random_engine re;
    // Getting a random double value
    double alpha = unif(re);

    double min_weight = heaviest_edge().second;
    double max_weight = lightest_edge().second;
    //cout<<"min_weight= "<<min_weight<<endl;
    //cout<<"max_weight= "<<max_weight<<endl;
    double weight_factor =  min_weight + alpha*(max_weight-min_weight);
    //cout<<"weight_factor= "<<weight_factor;
    vector<edge> RCL_edge ;

     for(long int i=0;i<num_of_vertices;i++)
    {
        for (long int j=0;j<num_of_vertices;j++)
        {
            if(Adj_Matt[i][j]!=inf  && Adj_Matt[i][j]!=0 && Adj_Matt[i][j]>=weight_factor)
            {
              RCL_edge.push_back(make_pair(make_pair(i,j),Adj_Matt[i][j]));
            }
        }
    }
    edge selected_edge = RCL_edge[rand()%RCL_edge.size()];
    set<long int> final_x;
    set<long int> final_y;
    

    final_x.insert(selected_edge.first.first);
    final_y.insert(selected_edge.first.second);
    

    //cout<<"Initial Addition:"<<"<"<<selected_edge.first.first<<","<<selected_edge.first.second<<">   "<<selected_edge.second<<endl;

    while(true)
    {
        set<long int> union_xy;
        set_union(final_x.begin(),final_x.end(),final_y.begin(),final_y.end(),inserter(union_xy,union_xy.end()));
        if(all_veritces == union_xy)
        {
            break;
        }
        set<long int> remaining_vertices;
        set_difference(all_veritces.begin(),all_veritces.end(),union_xy.begin(),union_xy.end(),inserter(remaining_vertices,remaining_vertices.end()));
        //cout<<"Remaining Vertices: ";
        // for(auto i: remaining_vertices)
        // {
        //     cout<<i<<endl;
        // }
        vector<double> sigma_x;
        vector<double> sigma_y;
        vector<long int> sigma_vertices;

        for(auto i: remaining_vertices)
        {
            double temp_x=0;
            double temp_y=0;
            for(auto j : final_y)
            {
                 if(Adj_Matt[i][j]!=inf  )
            {
                temp_x += Adj_Matt[i][j];
            } 

            }
            for(auto j : final_x)
            {
                 if(Adj_Matt[i][j]!=inf  )
            {
                temp_y += Adj_Matt[i][j];
            } 
            
            }
            //cout<<"Vertice :"<<i<<endl;
            //cout<<"Sigma_x :"<<temp_x<<endl;
            //cout<<"Sigma_y :"<<temp_y<<endl;
            
            sigma_x.push_back(temp_x);
            sigma_y.push_back(temp_y);
            sigma_vertices.push_back(i);    
        }
        double max_sigma_x=*max_element(sigma_x.begin(), sigma_x.end());
        double min_sigma_x=*min_element(sigma_x.begin(), sigma_x.end());
        double max_sigma_y=*max_element(sigma_y.begin(), sigma_y.end());
        double min_sigma_y=*min_element(sigma_y.begin(), sigma_y.end());

        min_weight = min(min_sigma_x,min_sigma_y);
        max_weight= max(max_sigma_x,max_sigma_y);

        alpha =  unif(re);
        weight_factor = min_weight + alpha*(max_weight-min_weight);

        vector<long int> RCL_vertices;
        for(long int i=0;i<remaining_vertices.size();i++)
        {
            if(max(sigma_x[i],sigma_y[i])>=weight_factor)
            {
                RCL_vertices.push_back(sigma_vertices[i]);
            }
        }
        long int selected_vertice = RCL_vertices[rand()%RCL_vertices.size()];
        //cout<<"Selected Vertice: "<<selected_vertice<<endl;
        long int selected_indx;
        for(long int i=0;i<sigma_vertices.size();i++)
        {
            if(selected_vertice==sigma_vertices[i])
            {
                selected_indx=i;
                break;
            }
        }
        //cout<<"Selected Vertice Index:"<<selected_indx<<"  found vertice at index: "<<sigma_vertices[selected_indx]<<endl;
        //cout<<"Sigma_x: "<<sigma_x[selected_indx]<<endl;
        //cout<<"Sigma_y: "<<sigma_y[selected_indx]<<endl;
        if(sigma_x[selected_indx]>=sigma_y[selected_indx])
        {
            final_x.insert(selected_vertice);
            
        }
        else
        {
            final_y.insert(selected_vertice);
            
        }

    }
    final.first.first=final_x;
    final.first.second=final_y;
    for(auto i : final.first.first)
    {
        for(auto j: final.first.second)
        {
            if(Adj_Matt[i][j]!=inf  && Adj_Matt[i][j]!=0)
            {
                final_weight+=Adj_Matt[i][j];
            }
        }
    }
    final.second = final_weight;
    return final;

}
cut greedy_maxcut()
{
    cut final;
    double final_weight=0;
    sort_by_edges_r();

    edge selected_edge = List_of_Edges[0];
    set<long int> final_x;
    set<long int> final_y;
    

    final_x.insert(selected_edge.first.first);
    final_y.insert(selected_edge.first.second);
    

        //cout<<"Initial Addition:"<<"<"<<selected_edge.first.first<<","<<selected_edge.first.second<<">   "<<selected_edge.second<<endl;

        set<long int> union_xy;
        set_union(final_x.begin(),final_x.end(),final_y.begin(),final_y.end(),inserter(union_xy,union_xy.end()));
        set<long int> remaining_vertices;
        set_difference(all_veritces.begin(),all_veritces.end(),union_xy.begin(),union_xy.end(),inserter(remaining_vertices,remaining_vertices.end()));
        //cout<<"Remaining Vertices: ";
        // for(auto i: remaining_vertices)
        // {
        //     cout<<i<<endl;
        // }

        for(auto i: remaining_vertices)
        {
            double sigma_x=0;
            double sigma_y=0;
            for(auto j : final_y)
            {
                 if(Adj_Matt[i][j]!=inf  )
            {
                sigma_x += Adj_Matt[i][j];
            } 

            }
            for(auto j : final_x)
            {
                 if(Adj_Matt[i][j]!=inf  )
            {
                sigma_y += Adj_Matt[i][j];
            } 
            
            }
            //cout<<"Vertice :"<<i<<endl;
            //cout<<"Sigma_x :"<<temp_x<<endl;
            //cout<<"Sigma_y :"<<temp_y<<endl;
             if(sigma_x>=sigma_y)
        {
            final_x.insert(i);
            
        }
        else
        {
            final_y.insert(i);
            
        }
   
        }
         
    final.first.first=final_x;
    final.first.second=final_y;
    for(auto i : final.first.first)
    {
        for(auto j: final.first.second)
        {
            if(Adj_Matt[i][j]!=inf  && Adj_Matt[i][j]!=0)
            {
                final_weight+=Adj_Matt[i][j];
            }
        }
    }
    final.second = final_weight;
    return final;

}
cut randomised_cut()
{
    cut final;
    double final_weight=0;
    set<long int> final_x;
    set<long int> final_y;

    edge selected_edge = List_of_Edges[rand()%num_of_edges];
    final_x.insert(selected_edge.first.first);
    final_y.insert(selected_edge.first.second);
           
      
        
        for(long int i=0;i<List_of_Edges.size();i++)
        {
            set<long int> union_xy;
            set_union(final_x.begin(),final_x.end(),final_y.begin(),final_y.end(),inserter(union_xy,union_xy.end()));
            if(rand()*i%2==0)
            {
                if(union_xy.count(List_of_Edges[i].first.first)==0)
                {
                    final_x.insert(List_of_Edges[i].first.first);
                }
                if(union_xy.count(List_of_Edges[i].first.second)==0)
                {
                    final_y.insert(List_of_Edges[i].first.second);
                }    
            }
            else
            {
                if(union_xy.count(List_of_Edges[i].first.first)==0)
                {
                    final_y.insert(List_of_Edges[i].first.first);
                }
                if(union_xy.count(List_of_Edges[i].first.second)==0)
                {
                    final_x.insert(List_of_Edges[i].first.second);
                }
                
            }
        }

        set<long int> union_xy;
        set_union(final_x.begin(),final_x.end(),final_y.begin(),final_y.end(),inserter(union_xy,union_xy.end()));

        if(all_veritces!=union_xy)
        {
        set<long int> remaining_vertices;
        set_difference(all_veritces.begin(),all_veritces.end(),union_xy.begin(),union_xy.end(),inserter(remaining_vertices,remaining_vertices.end()));
        for(auto i:remaining_vertices)
        {
             if(rand()%2==0)
            {
                final_x.insert(i);
                
            }
            else
            {
                final_y.insert(i);
                
            }
        }
        }

    final.first.first=final_x;
    final.first.second=final_y;
    for(auto i : final.first.first)
    {
        for(auto j: final.first.second)
        {
            if(Adj_Matt[i][j]!=inf  && Adj_Matt[i][j]!=0)
            {
                final_weight+=Adj_Matt[i][j];
            }
        }
    }
    final.second = final_weight;
    return final;

    
    
}
cut randomised_cutv2()
{
    cut final;
    double final_weight=0;
    set<long int> final_x;
    set<long int> final_y;

           
      
        
        for(auto i : all_veritces)
        {
            set<long int> union_xy;
            set_union(final_x.begin(),final_x.end(),final_y.begin(),final_y.end(),inserter(union_xy,union_xy.end()));
            if(rand()%2==0)
            {
                final_x.insert(i);
            }
            else
            {
                final_y.insert(i);
                
            }
        }

    final.first.first=final_x;
    final.first.second=final_y;
    for(auto i : final.first.first)
    {
        for(auto j: final.first.second)
        {
            if(Adj_Matt[i][j]!=inf  && Adj_Matt[i][j]!=0)
            {
                final_weight+=Adj_Matt[i][j];
            }
        }
    }
    final.second = final_weight;
    return final;

    
    
}
cut local_search_maxcut(cut input)
{
    cut final=input;
    bool change =true;
    double final_weight=0;

    while (change)
    {
        change = false;
            for(auto i : all_veritces)
            {
                if(change == true)
                {
                    break;
                }
                double sigma_s=0;
                double sigma_s_prime=0;
                for(auto j : final.first.second)
                {
                    if(Adj_Matt[i][j]!=inf  && Adj_Matt[i][j]!=0)
                    {
                        sigma_s+=Adj_Matt[i][j];
                    }
                }
                for(auto j : final.first.first)
                {
                    if(Adj_Matt[i][j]!=inf  && Adj_Matt[i][j]!=0)
                    {
                        sigma_s_prime+=Adj_Matt[i][j];
                    }
                }
                if(final.first.first.count(i)==1 && sigma_s_prime>sigma_s)
                {
                    final.first.first.erase(i);
                    final.first.second.insert(i);
                    change=true;
                }
                else if(final.first.second.count(i)==1 && sigma_s>sigma_s_prime)
                {
                    final.first.second.erase(i);
                    final.first.first.insert(i);
                    change=true;
                }
            }
    }
    for(auto i : final.first.first)
    {
        for(auto j: final.first.second)
        {
            if(Adj_Matt[i][j]!=inf  && Adj_Matt[i][j]!=0)
            {
                final_weight+=Adj_Matt[i][j];
            }
        }
    }
    final.second=final_weight;
    return final;
}
cut grasp_maxcut(int iteration_count,int greedy_choice)
{
    double max_weight = -inf;
    cut final_cut; 
    double prev_weight,after_weight;
    for(int i=1;i<=iteration_count;i++)
    {
        cout<<"iteration: "<<i<<endl;
        cut temp_cut;
        if(greedy_choice==1)
        {
            temp_cut = greedy_maxcut();
        }
        else if(greedy_choice==2)
        {

            temp_cut =  semi_greedy_maxcut();
        }
        else if(greedy_choice==3)
        {
            temp_cut = randomised_cut();
        }
        else if(greedy_choice==4)
        {
            temp_cut = randomised_cutv2();
        }

        
        prev_weight=temp_cut.second;
        //cout<<"Prev_Weight"<<prev_weight<<endl;
        temp_cut = local_search_maxcut(temp_cut);
        after_weight=temp_cut.second;
        //cout<<"After_Weight"<<after_weight<<endl;
        if(temp_cut.second>=max_weight)
        {
            final_cut=temp_cut;
            max_weight=temp_cut.second;
        }
    } 
    return final_cut;
}
cut grasp_maxcut(int greedy_choice)
{
    double max_weight = -inf;
    cut final_cut;
    double prev_weight,after_weight;
    long int iteration_count=0; 
    while(true)
    {
        iteration_count++;
        cout<<"iteration: "<<iteration_count<<endl;
        cut temp_cut;
        if(greedy_choice==1)
        {
            temp_cut = greedy_maxcut();
        }
        else if(greedy_choice==2)
        {

            temp_cut =  semi_greedy_maxcut();
        }
        else if(greedy_choice==3)
        {
            temp_cut = randomised_cut();
        }
        else if(greedy_choice==4)
        {
            temp_cut = randomised_cutv2();
        }
        
        prev_weight=temp_cut.second;
        //cout<<"Prev_Weight"<<prev_weight<<endl;
        temp_cut = local_search_maxcut(temp_cut);
        after_weight=temp_cut.second;
        //cout<<"After_Weight"<<after_weight<<endl;
        if(temp_cut.second>=max_weight)
        {
            final_cut=temp_cut;
            max_weight=temp_cut.second;
        }
        if(prev_weight == after_weight)
        {
            break;
        }
    } 
    return final_cut;
}


~Graph()
    {
        for(long int i=0; i<num_of_vertices; i++)
        {
            
            delete[] Adj_Matt[i];
        }
        delete[]Adj_Matt;
    }


};
int main()
{
    // int num=1;
    // string filepath = "set1/g"+to_string(num)+".rud";
    // fstream infile(filepath, std::ios_base::in);
    fstream infile("input.txt",std::ios_base::in);
    fstream outfile("output.txt",std::ios_base::out);
    int num_v,num_edge;
    infile>>num_v>>num_edge;
    //cin>>num_v>>num_edge;
    //cout<<num_v<<"    "<<num_edge<<endl;
    Graph X(num_v,num_edge);
    for(long int i=0;i<num_edge;i++)
    {   
        long int u,v;
        double w;
        infile>>u>>v>>w;
        //cin>>u>>v>>w;
        //cout<<u<<"   "<<v<<"   "<<w<<endl;
        X.Add_Edge_Undirected(u-1,v-1,w);
       }
       //X.print_AdjM();
       //edge e = X.heaviest_edge();
       //edge e = X.lightest_edge();
       //cout<<e.first.first<<"----"<<e.first.second<<"=="<<e.second<<endl;
        cut temp;
        //temp = X.semi_greedy_maxcut();
        //temp = X.local_search_maxcut(temp);
        //temp = X.grasp_maxcut(20,1);
        //temp = X.greedy_maxcut();
        //temp = X.randomised_cut();
        //temp = X.randomised_cutv2();
       outfile<<"S  :"<<endl;
       for(auto i : temp.first.first)
       {
        outfile<<i+1<<endl;
       }
       outfile<<"S_prime  :"<<endl;
       for(auto i : temp.first.second)
       {
        outfile<<i+1<<endl;
       }
       outfile<<"Max_Weight  : "<<temp.second<<endl;
    return 0;
}
