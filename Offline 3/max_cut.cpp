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
#include<chrono>
using namespace std;
typedef pair<long int,double> dest_weight;
typedef pair<long int,long int> vertice;
typedef pair<vertice,double> edge;
typedef pair<pair<set<long int>,set<long int>>,double> cut;
typedef pair<double,double> weight_itr;
typedef pair<cut,int> cut_itr;
#define inf 9999999.9999999

fstream logfile("log.txt",std::ios_base::out);

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
    vector<edge> sort_by_edges()//Order of E(square)
    {
        vector<edge> ret=List_of_Edges;
        for(long int i=0; i<num_of_edges; i++)
        {
            for(long int j=i+1; j<num_of_edges; j++)
            {
                if(ret[i].second>=ret[j].second)
                {
                    pair<vertice,double> temp;
                    temp=ret[i];
                    ret[i]=ret[j];
                    ret[j]=temp;
                }
            }
        }
        return ret;
    }
    vector<edge> sort_by_edges_r()//Order of E(square)
    {
        vector<edge> ret = List_of_Edges;
        for(long int i=0; i<num_of_edges; i++)
        {
            for(long int j=i+1; j<num_of_edges; j++)
            {
                if(ret[i].second<=ret[j].second)
                {
                    pair<vertice,double> temp;
                    temp=ret[i];
                    ret[i]=ret[j];
                    ret[j]=temp;
                }
            }
        }
        return ret;
    }
    
    void Print_Edges(vector<edge> test)//Order of V
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
    vector<edge> sorted_edgelist=sort_by_edges_r();

    edge selected_edge = sorted_edgelist[0];
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
        double sigma_x;
        double sigma_y;
        for(auto i: remaining_vertices)
        {
            sigma_x=0;
            sigma_y=0;
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
cut greedy_maxcutv2()
{
    cut final;
    double final_weight=0;
    vector<edge> sorted_edgelist=sort_by_edges_r();

    set<long int> final_x;
    set<long int> final_y;
    final_x.insert(sorted_edgelist[0].first.first);
    final_y.insert(sorted_edgelist[0].first.second);
    

        for(int i=1;i<sorted_edgelist.size();i++)
        {
            double sigma_x;
            double sigma_y;
            set<long int> union_xy;
            set_union(final_x.begin(),final_x.end(),final_y.begin(),final_y.end(),inserter(union_xy,union_xy.end()));
            if(all_veritces==union_xy)
            {
                break;
            }
            
        
        if(union_xy.count(sorted_edgelist[i].first.first)==0)
        {
        
            sigma_x=0;
            sigma_y=0;
            for(auto j : final_y)
            {
                 if(Adj_Matt[sorted_edgelist[i].first.first][j]!=inf  )
            {
                sigma_x += Adj_Matt[sorted_edgelist[i].first.first][j];
            } 

            }
            for(auto j : final_x)
            {
                 if(Adj_Matt[sorted_edgelist[i].first.first][j]!=inf  )
            {
                sigma_y += Adj_Matt[sorted_edgelist[i].first.first][j];
            } 
            
            }
            //cout<<"Vertice :"<<i<<endl;
            //cout<<"Sigma_x :"<<temp_x<<endl;
            //cout<<"Sigma_y :"<<temp_y<<endl;
             if(sigma_x>=sigma_y)
        {
            final_x.insert(sorted_edgelist[i].first.first);
            
        }
        else
        {
            final_y.insert(sorted_edgelist[i].first.first);
            
        }
            
        }
        if(union_xy.count(sorted_edgelist[i].first.second)==0)
        {
          
             sigma_x=0;
             sigma_y=0;
            for(auto j : final_y)
            {
                 if(Adj_Matt[sorted_edgelist[i].first.second][j]!=inf  )
            {
                sigma_x += Adj_Matt[sorted_edgelist[i].first.second][j];
            } 

            }
            for(auto j : final_x)
            {
                 if(Adj_Matt[sorted_edgelist[i].first.second][j]!=inf  )
            {
                sigma_y += Adj_Matt[sorted_edgelist[i].first.second][j];
            } 
            
            }
            //cout<<"Vertice :"<<i<<endl;
            //cout<<"Sigma_x :"<<temp_x<<endl;
            //cout<<"Sigma_y :"<<temp_y<<endl;
             if(sigma_x>=sigma_y)
        {
            final_x.insert(sorted_edgelist[i].first.second);
            
        }
        else
        {
            final_y.insert(sorted_edgelist[i].first.second);
            
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
cut randomised_cutv3()
{
    cut final;
    double final_weight=0;
    set<long int> final_x;
    set<long int> final_y;
    vector<edge> sorted_edgelist=sort_by_edges();
    edge selected_edge = sorted_edgelist[num_of_edges-1];
    final_x.insert(selected_edge.first.first);
    final_y.insert(selected_edge.first.second);
           
      
        
        for(long int i=0;i<sorted_edgelist.size()-1;i++)
        {
            set<long int> union_xy;
            set_union(final_x.begin(),final_x.end(),final_y.begin(),final_y.end(),inserter(union_xy,union_xy.end()));
            if(rand()%2==0)
            {
                if(union_xy.count(sorted_edgelist[i].first.first)==0)
                {
                    final_x.insert(sorted_edgelist[i].first.first);
                }
                if(union_xy.count(sorted_edgelist[i].first.second)==0)
                {
                    final_y.insert(sorted_edgelist[i].first.second);
                }    
            }
            else
            {
                if(union_xy.count(sorted_edgelist[i].first.first)==0)
                {
                    final_y.insert(sorted_edgelist[i].first.first);
                }
                if(union_xy.count(sorted_edgelist[i].first.second)==0)
                {
                    final_x.insert(sorted_edgelist[i].first.second);
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
cut_itr local_search_maxcut(cut input)
{
    cut final=input;
    bool change =true;
    double final_weight=0;
    int iteration_count=0;

    while (change)
    {
        change = false;
        iteration_count++;
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
    return make_pair(final,iteration_count);
}

pair<cut,weight_itr> grasp_maxcut(int iteration_count,int greedy_choice)
{
    double max_weight = -inf;
    cut final_cut; 
    double prev_weight,after_weight;
    int i;
    double sum_weight=0;
    double sum_itr=0; 
    for(i=1;i<=iteration_count;i++)
    {
        logfile<<"iteration: "<<i<<endl;
        cout<<"iteration: "<<i<<endl;
        cut temp_cut;
        pair<cut,int> temp_cut_itr;
        if(greedy_choice==1)
        {
            temp_cut = greedy_maxcut();
        }
        else if(greedy_choice==2)
        {
            temp_cut = greedy_maxcutv2();
        }
        else if(greedy_choice==3)
        {

            temp_cut =  semi_greedy_maxcut();
        }
        else if(greedy_choice==4)
        {
            temp_cut = randomised_cut();
        }
        else if(greedy_choice==5)
        {
            temp_cut = randomised_cutv2();
        }
        else if(greedy_choice==6)
        {
            temp_cut = randomised_cutv3();
        }

        
        prev_weight=temp_cut.second;
        //cout<<"Prev_Weight"<<prev_weight<<endl;
        temp_cut_itr = local_search_maxcut(temp_cut);
        after_weight=temp_cut_itr.first.second;
        //cout<<"After_Weight"<<after_weight<<endl;
        sum_weight+=temp_cut_itr.first.second;
        sum_itr+=temp_cut_itr.second;
        if(temp_cut_itr.first.second>=max_weight)
        {
            final_cut=temp_cut_itr.first;
            max_weight=temp_cut_itr.first.second;
        }
        if(prev_weight == after_weight)
        {
            break;
        }
    } 
    weight_itr ret_weight_itr;
    ret_weight_itr.first=sum_weight/(iteration_count-1)*1.0;
    ret_weight_itr.second=sum_itr/(iteration_count-1)*1.0;
   // outfile<<"The number of iteration completed: "<<i-1<<endl;
    return make_pair(final_cut,ret_weight_itr);
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
    int num=10;
    int n=10;
    int iteration_count=100;
    int const_algo_count=6;
    int graph_number;
    fstream outfile("constructive.txt",std::ios_base::out);
    fstream outcsv("constructive.csv",std::ios_base::out);
    fstream outfileg("grasp.txt",std::ios_base::out);
    fstream outcsvg("grasp.csv",std::ios_base::out);
    string filepath;
    outfile<<"Problem"<<"\tVertices"<<"\tEdges"<<"\tGreedy"<<"\tGreedy2"<<"\tSemi-Greedy"<<"\tRandomised"<<"\tRandomised2"<<"\tRandomised3"<<endl;
    outcsv<<"Problem,"<<"Vertices,"<<"Edges,"<<"Greedy,"<<"Greedy2,"<<"Semi-Greedy,"<<"Randomised,"<<"Randomised2,"<<"Randomised3"<<endl;
    outfileg<<"Problem"<<"\tVertices"<<"\tEdges"<<"\tConstruction"<<"\tLocal Search Iteration"<<"\tLocal Search Best"<<"\tGrasp Iteration"<<"\t\tGrasp Best"<<endl;
    outcsvg<<"Problem"<<",Vertices"<<",Edges"<<",Construction"<<",Local Search Iteration"<<",Local Search Best"<<",Grasp Iteration"<<",Grasp Best"<<endl;
    int num_v,num_edge;
    for(int i=1;i<=num;i++)
    {
        graph_number=i;
        logfile<<"Simulating G"<<graph_number<<endl;
        cout<<"Simulating G"<<graph_number<<endl;
        filepath = "set1/g"+to_string(graph_number)+".rud";
        fstream infile(filepath,std::ios_base::in);
        infile>>num_v>>num_edge;
        outfile<<"G"<<graph_number<<"\t\t\t"<<num_v<<"\t\t"<<num_edge;
        outcsv<<"G"<<graph_number<<","<<num_v<<","<<num_edge;
        Graph X(num_v,num_edge);
    for(long int i=0;i<num_edge;i++)
    {   
        long int u,v;
        double w;
        infile>>u>>v>>w;
        X.Add_Edge_Undirected(u-1,v-1,w);
    }
    logfile<<"Constructed Graph"<<graph_number<<endl;
    cout<<"Constructed Graph"<<graph_number<<endl;
    cut result;
    logfile<<"Running Greedy"<<endl;
    cout<<"Running Greedy"<<endl;
    result=X.greedy_maxcut();
    outfile<<"\t"<<result.second;
    outcsv<<","<<result.second;
    logfile<<"Running Greedy2"<<endl;
    cout<<"Running Greedy2"<<endl;
    result=X.greedy_maxcutv2();
    outfile<<"\t"<<result.second<<"\t";
    outcsv<<","<<result.second;

    long int sum_cut=0;
    for(int i=0;i<n;i++)
    {
        logfile<<"Running Semi-Greedy -> "<<i+1<<endl;
        cout<<"Running Semi-Greedy -> "<<i+1<<endl;
        result=X.semi_greedy_maxcut();
        sum_cut+=result.second;
    }
    outfile<<"\t"<<sum_cut/n<<"\t\t";
    outcsv<<","<<sum_cut/n;
    sum_cut=0;
    for(int i=0;i<n;i++)
    {
        logfile<<"Running Randomised -> "<<i+1<<endl;
        cout<<"Running Randomised -> "<<i+1<<endl;
        result=X.randomised_cut();
        sum_cut+=result.second;
    }
    outfile<<sum_cut/n<<"\t\t\t";
    outcsv<<","<<sum_cut/n;
    sum_cut=0;
    for(int i=0;i<n;i++)
    {
        logfile<<"Running Randomised2 -> "<<i+1<<endl;
        cout<<"Running Randomised2 -> "<<i+1<<endl;
        result=X.randomised_cutv2();
        sum_cut+=result.second;
    }
    outfile<<sum_cut/n<<"\t\t";
    outcsv<<","<<sum_cut/n;
    sum_cut=0;
    for(int i=0;i<n;i++)
    {
        logfile<<"Running Randomised3 -> "<<i+1<<endl;
        cout<<"Running Randomised3 -> "<<i+1<<endl;
        result=X.randomised_cutv3();
        sum_cut+=result.second;
    }
    outfile<<sum_cut/n<<endl;
    outcsv<<","<<sum_cut/n<<endl;
    for(int i=1;i<=6;i++)
    {
    outfileg<<"G"<<graph_number<<"\t\t\t"<<num_v<<"\t\t"<<num_edge;
    outcsvg<<"G"<<graph_number<<","<<num_v<<","<<num_edge;
    if(i==1)
    {
    outfileg<<"\t\tGreedy";
    outcsvg<<",Greedy"; 
    logfile<<"Running Grasp with iterations :"<<iteration_count<<" & construction :Greedy"<<endl;
    cout<<"Running Grasp with iterations :"<<iteration_count<<" & construction :Greedy"<<endl;
    }
    else if(i==2)
    {
    outfileg<<"\t\tGreedy2";
    outcsvg<<",Greedy2"; 
    logfile<<"Running Grasp with iterations :"<<iteration_count<<" & construction :Greedy2"<<endl;
    cout<<"Running Grasp with iterations :"<<iteration_count<<" & construction :Greedy2"<<endl;
    }
    else if(i==3)
    {
    outfileg<<"\t\tSemi-Greedy";
    outcsvg<<",Semi-Greedy"; 
    logfile<<"Running Grasp with iterations :"<<iteration_count<<" & construction :Semi-Greedy"<<endl;
    cout<<"Running Grasp with iterations :"<<iteration_count<<" & construction :Semi-Greedy"<<endl;
    }
    else if(i==4)
    {
    outfileg<<"\t\tRandomised";
    outcsvg<<",Randomised"; 
    logfile<<"Running Grasp with iterations :"<<iteration_count<<" & construction :Randomised"<<endl;
    cout<<"Running Grasp with iterations :"<<iteration_count<<" & construction :Randomised"<<endl;
    }
    else if(i==5)
    {
    outfileg<<"\t\tRandomised2";
    outcsvg<<",Randomised2"; 
    logfile<<"Running Grasp with iterations :"<<iteration_count<<" & construction :Randomised2"<<endl;
    cout<<"Running Grasp with iterations :"<<iteration_count<<" & construction :Randomised2"<<endl;
    }
    else if(i==6)
    {
    outfileg<<"\t\tRandomised3";
    outcsvg<<",Randomised3"; 
    logfile<<"Running Grasp with iterations :"<<iteration_count<<" & construction :Randomised3"<<endl;
    cout<<"Running Grasp with iterations :"<<iteration_count<<" & construction :Randomised3"<<endl;
    }
    pair<cut,weight_itr> final;
    final = X.grasp_maxcut(iteration_count,i);
    outfileg<<"\t\t"<<final.second.second<<"\t\t\t\t\t"<<final.second.first<<"\t\t\t\t\t"<<iteration_count<<"\t\t\t\t\t"<<final.first.second<<endl;
    outcsvg<<","<<final.second.second<<","<<final.second.first<<","<<iteration_count<<","<<final.first.second<<endl;
    }
    
    }


    //     fstream infile("set1/g1.rud",std::ios_base::in);
    //     long int num_v,num_edge;
    //     infile>>num_v>>num_edge;
    //     Graph X(num_v,num_edge);
    // for(long int i=0;i<num_edge;i++)
    // {   
    //     long int u,v;
    //     double w;
    //     infile>>u>>v>>w;
    //     X.Add_Edge_Undirected(u-1,v-1,w);
    // }
    // pair<cut,weight_itr> res = X.grasp_maxcut(1000,3);
 
    return 0;
}
