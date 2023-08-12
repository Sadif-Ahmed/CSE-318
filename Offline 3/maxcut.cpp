
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
typedef pair<int,double> dest_weight;
typedef pair<int,int> vertice;
typedef pair<vertice,double> edge;
typedef pair<pair<set<int>,set<int>>,double> cut;
#define inf 9999999.9999999
class Graph
{
public:
    int num_of_vertices;
    int num_of_edges;
    vector<dest_weight> *dir_graph;
    vector<dest_weight> *rev_dir_graph;
    enum colour
    {
        WHITE,
        RED,
        BLUE,
        GRAY,
        BLACK
    };
    vector<int>bfs_parent;
    vector<int>dfs_parent;
    vector<double>bfs_src_distances;
    vector<int>bfs_visit_count;
    int dfs_time=0;
    vector<int>dfs_disc_t;
    vector<int>dfs_end_t;
    stack<int> reverse_topological;
    queue<int> bfs_queue;
    vector<bool>und_cycle_visit;
    vector<colour>bipart;
    vector<colour>visit_status;
    vector<bool>visit_status_dir;
    vector<bool>visit_status_rev_dir;
    //MST Elements
    vector<int> sources;
    vector<pair<vertice,double>> List_of_Edges;//Storing the edges
    vector<pair<vertice,double>> Kruskal_MST_Edges;//Storing the mst
    double cost_k;
    double cost_p;
    vector<double>key;//Vector of Weights
    vector<int> parent;//Prim's MST parent List
    vector<bool>mst_status;//Prim's node in MST checker

    double **Adj_Matt;
    double **Result_Mat;
    int **Predecessor; 

    //maxcut
    set<int> all_veritces;

    Graph(int num_v,int num_e)
    {
        num_of_vertices=num_v;
        num_of_edges=num_e;
        dir_graph= new vector<dest_weight>[num_of_vertices];
        rev_dir_graph= new vector<dest_weight>[num_of_vertices];
        bfs_parent.resize(num_of_vertices,-1);
        dfs_parent.resize(num_of_vertices,-1);
        bfs_src_distances.resize(num_of_vertices,INT_MAX);
        bfs_visit_count.resize(num_of_vertices,INT_MAX);
        dfs_disc_t.resize(num_of_vertices,0);
        dfs_end_t.resize(num_of_vertices,0);
        und_cycle_visit.resize(num_of_vertices,false);
        bipart.resize(num_of_vertices,WHITE);
        visit_status.resize(num_of_vertices,WHITE);
        visit_status_dir.resize(num_of_vertices,false);
        visit_status_rev_dir.resize(num_of_vertices,false);

        cost_k=0;
        cost_p=0;
        sources.resize(num_of_vertices,0);
        for(int i=0; i<num_of_vertices; i++)
        {
            sources[i]=i;
        }
        List_of_Edges.clear();
        Kruskal_MST_Edges.clear();
        key.resize(num_of_vertices,inf);
        parent.resize(num_of_vertices,-1);
        mst_status.resize(num_of_vertices,false);

        Adj_Matt=new double*[num_of_vertices];
        for(int i=0; i<num_of_vertices; i++)
        {
            Adj_Matt[i]=new double[num_of_vertices];
        }
        for(int i=0; i<num_of_vertices; i++)
        {
            for(int j=0; j<num_of_vertices; j++)
            {
                if(i==j)
                {
                    Adj_Matt[i][j]=0;
                }
                else
                {
                    Adj_Matt[i][j]=INT_MAX;
                }
            }
        }
        Predecessor=new int*[num_of_vertices];
        for(int i=0; i<num_of_vertices; i++)
        {
            Predecessor[i]=new int[num_of_vertices];
        }
        for(int i=0; i<num_of_vertices; i++)
        {
            for(int j=0; j<num_of_vertices; j++)
            {
                Predecessor[i][j]=-1;
            }
        }


        for(int i=0;i<num_of_vertices;i++)
        {
            all_veritces.insert(i);
        }

    }
    void reset()
    {
        bfs_parent.clear();
        dfs_parent.clear();
        bfs_src_distances.clear();
        bfs_visit_count.clear();
        dfs_disc_t.clear();
        dfs_end_t.clear();
        und_cycle_visit.clear();
        bipart.clear();
        visit_status.clear();
        visit_status_dir.clear();
        visit_status_rev_dir.clear();
        bfs_parent.resize(num_of_vertices,-1);
        dfs_parent.resize(num_of_vertices,-1);
        bfs_src_distances.resize(num_of_vertices,INT_MAX);
        bfs_visit_count.resize(num_of_vertices,INT_MAX);
        dfs_disc_t.resize(num_of_vertices,0);
        dfs_end_t.resize(num_of_vertices,0);
        und_cycle_visit.resize(num_of_vertices,false);
        bipart.resize(num_of_vertices,WHITE);
        visit_status.resize(num_of_vertices,WHITE);
        visit_status_dir.resize(num_of_vertices,false);
        visit_status_rev_dir.resize(num_of_vertices,false);

        cost_k=0;
        cost_p=0;
        sources.resize(num_of_vertices,0);
        for(int i=0; i<num_of_vertices; i++)
        {
            sources[i]=i;
        }
        Kruskal_MST_Edges.clear();
        key.resize(num_of_vertices,inf);
        parent.resize(num_of_vertices,-1);
        mst_status.resize(num_of_vertices,false);
    }
    vector<pair<vertice,double>> get_mst()
    {
        return Kruskal_MST_Edges;
    }
    vector<pair<vertice,double>> get_graph_edges()
    {
        return List_of_Edges;
    }
    double get_cost(char algo)
    {
        if(algo=='k')
            return cost_k;
        else
            return cost_p;
    }
    vector<int> get_parent()
    {
        return parent;
    }
    vector<double> get_weight()
    {
        return key;
    }
    double ** get_APSP()
    {
        return Result_Mat;
    }
    int ** get_Predecessor()
    {
        return Predecessor;
    }
    double ** get_mat()
    {
        return Adj_Matt;
    }
    void Add_Edge_Directed(int startv,int endv,double weight=0)
    {
        dir_graph[startv].push_back(make_pair(endv,weight));
        rev_dir_graph[endv].push_back(make_pair(startv,weight));
        List_of_Edges.push_back(make_pair(make_pair(startv,endv),weight));
        Adj_Matt[startv][endv]=weight;
        Predecessor[startv][endv]=endv;

    }
    void Add_Edge_Undirected(int startv,int endv,double weight=0)
    {
        dir_graph[startv].push_back(make_pair(endv,weight));
        dir_graph[endv].push_back(make_pair(startv,weight));
        List_of_Edges.push_back(make_pair(make_pair(startv,endv),weight));
        List_of_Edges.push_back(make_pair(make_pair(endv,startv),weight));
        Adj_Matt[startv][endv]=weight;
        Adj_Matt[endv][startv]=weight;
        Predecessor[startv][endv]=endv;
        Predecessor[endv][startv]=startv;
    }
    void Remove_Edge_Directed(int startv,int endv,double weight=0)
    {

        for(int i=0; i<List_of_Edges.size(); i++)
        {
            if(List_of_Edges[i].first.first==startv&&List_of_Edges[i].first.second==endv&&List_of_Edges[i].second==weight)
            {
                List_of_Edges.erase(List_of_Edges.begin()+i);
            }
        }
        for(int j=0; j<dir_graph[startv].size(); j++)
        {
            if(dir_graph[startv][j].first==endv&&dir_graph[startv][j].second==weight)
            {
                dir_graph[startv].erase(dir_graph[startv].begin()+j);
            }
        }
        for(int j=0; j<rev_dir_graph[endv].size(); j++)
        {
            if(rev_dir_graph[endv][j].first==startv&&rev_dir_graph[endv][j].second==weight)
            {
                rev_dir_graph[endv].erase(rev_dir_graph[endv].begin()+j);
            }
        }
        Adj_Matt[startv][endv]=INT_MAX;
        Predecessor[startv][endv]=-1;

    }
    void Remove_Edge_UnDirected(int startv,int endv,int weight=0)
    {

        for(int i=0; i<List_of_Edges.size(); i++)
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
        for(int j=0; j<dir_graph[startv].size(); j++)
        {
            if(dir_graph[startv][j].first==endv&&dir_graph[startv][j].second==weight)
            {
                dir_graph[startv].erase(dir_graph[startv].begin()+j);
            }
        }
        for(int j=0; j<dir_graph[endv].size(); j++)
        {
            if(dir_graph[endv][j].first==startv&&dir_graph[endv][j].second==weight)
            {
                dir_graph[endv].erase(dir_graph[endv].begin()+j);
            }
        }
        Adj_Matt[startv][endv]=INT_MAX;
        Adj_Matt[endv][startv]=INT_MAX;
        Predecessor[startv][endv]=-1;
        Predecessor[endv][startv]=-1;

    }
    double find_cost(int u,int v)
    {
        for(int i=0; i<List_of_Edges.size(); i++)
        {
            if(List_of_Edges[i].first.first==u&&List_of_Edges[i].first.second==v)
            {
                return List_of_Edges[i].second;
            }
        }
        return inf;

    }
    bool check_edge_existence(int u,int v,double w)
    {
        for(int i=0; i<List_of_Edges.size(); i++)
        {
            if(List_of_Edges[i].first.first==u&&List_of_Edges[i].first.second==v&&List_of_Edges[i].second==w)
            {
                return true;
            }
        }
        return false;
    }

    void print_AdjL()
    {
        cout<<"Adjacency List:"<<endl;
        for(int i=0; i<num_of_vertices; i++)
        {
            cout<<i<<"->"<<" ";
            for(vector<dest_weight>::iterator it=dir_graph[i].begin(); it!=dir_graph[i].end(); it++)
            {
                cout<<(*it).first<<"(w= "<<(*it).second<<" )";
            }
            cout<<endl;
        }
    }
    void print_AdjM()
    {
        cout<<"Adjacency Matrix"<<endl;
        for(int i=0; i<num_of_vertices; i++)
        {
            for(int j=0; j<num_of_vertices; j++)
            {
                if(Adj_Matt[i][j]==INT_MAX)
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
    void BFS_Unweighted(int root)
    {
        visit_status[root]=GRAY;
        bfs_src_distances[root]=0;
        bfs_parent[root]=0;

        bfs_queue.push(root);
        while(!bfs_queue.empty())
        {
            int temp=bfs_queue.front();
            bfs_queue.pop();
            cout<<temp<<" ";

            for(vector<dest_weight>::iterator it=dir_graph[temp].begin(); it!=dir_graph[temp].end(); it++)
            {
                if(visit_status[(*it).first]==WHITE)
                {
                    visit_status[(*it).first]=GRAY;
                    bfs_src_distances[(*it).first]=bfs_src_distances[root]+1;
                    bfs_parent[(*it).first]=temp;
                    bfs_queue.push((*it).first);
                }
            }
            visit_status[temp]=BLACK;
        }

    }
    void DFS_Unweighted(int root)
    {
        dfs_time++;
        dfs_disc_t[root]=dfs_time;
        visit_status[root]=GRAY;
        cout<<root<<" ";
        for(vector<dest_weight>::iterator it=dir_graph[root].begin(); it!=dir_graph[root].end(); it++)
        {
            if(visit_status[(*it).first]==WHITE)
            {

                dfs_parent[(*it).first]=root;
                DFS_Unweighted((*it).first);
            }
        }
        visit_status[root]=BLACK;

        dfs_time++;
        dfs_end_t[root]=dfs_time;

    }
    void Traverse_Choice(char c)
    {
        reset();
        for(int i=0; i<num_of_vertices; i++)
        {
            if(visit_status[i]==WHITE)
            {
                if(c=='B')
                {
                    BFS_Unweighted(i);
                    cout<<endl;
                }
                else if(c=='D')
                {
                    DFS_Unweighted(i);
                    cout<<endl;
                }
                else
                {
                    cout<<"Invalid Choice"<<endl;
                }
            }
        }
    }
    bool pre_cycle_check_Dir(int vert)
    {
        visit_status[vert]=GRAY;
        for(vector<dest_weight>::iterator it=dir_graph[vert].begin(); it!=dir_graph[vert].end(); it++)
        {
            if(visit_status[(*it).first]==GRAY)
            {
                return true;
            }
            if((visit_status[(*it).first]==WHITE)&&(pre_cycle_check_Dir((*it).first)))
            {
                return true;
            }
        }
        visit_status[vert]=BLACK;
        return false;
    }
    bool pre_cycle_check_Undir(int vert,int parent)
    {
        und_cycle_visit[vert]=true;
        for(vector<dest_weight>::iterator it=dir_graph[vert].begin(); it!=dir_graph[vert].end(); it++)
        {
            if(!(und_cycle_visit[(*it).first]))
            {
                if(pre_cycle_check_Undir((*it).first,vert))
                {
                    return true;
                }
            }
            else if((*it).first!=parent)
            {
                return true;
            }

        }
        return false;
    }
    bool dir_cycle_check()
    {
        reset();
        for(int i=0; i<num_of_vertices; i++)
        {
            if(visit_status[i]==WHITE&&pre_cycle_check_Dir(i))
            {
                return true;
            }
        }
        return false;

    }
    bool undir_cycle_check()
    {
        reset();
        for(int i=0; i<num_of_vertices; i++)
        {
            if(!und_cycle_visit[i])
            {
                if(pre_cycle_check_Undir(i,-1))
                {
                    return true;
                }
            }
        }
        return false;
    }
    void pre_topological_sort(int vert)
    {
        visit_status[vert]=BLACK;
        for(vector<dest_weight>::iterator it=dir_graph[vert].begin(); it!=dir_graph[vert].end(); it++)
        {
            if(visit_status[(*it).first]!=BLACK)
            {
                pre_topological_sort((*it).first);
            }
        }
        reverse_topological.push(vert);

    }
    void topological_sort()
    {
        reset();
        for(int i=0; i<num_of_vertices; i++)
        {
            if(visit_status[i]!=BLACK)
            {
                pre_topological_sort(i);
            }
        }
        while(!reverse_topological.empty())
        {
            cout<<reverse_topological.top()<<"  ";
            reverse_topological.pop();
        }
    }
    void dir_dfs(int root)
    {
        visit_status_dir[root]=true;
        for(vector<dest_weight>::iterator it=dir_graph[root].begin(); it!=dir_graph[root].end(); it++)
        {
            if(!visit_status_dir[(*it).first])
            {
                dir_dfs((*it).first);
            }
        }
    }
    void rev_dir_dfs(int root)
    {
        visit_status_rev_dir[root]=true;
        for(vector<dest_weight>::iterator it=rev_dir_graph[root].begin(); it!=rev_dir_graph[root].end(); it++)
        {
            if(!visit_status_rev_dir[(*it).first])
            {
                rev_dir_dfs((*it).first);
            }
        }
    }
    bool undir_connected()
    {
        reset();
        dir_dfs(0);
        rev_dir_dfs(0);
        for(int i=1; i<=num_of_vertices; i++)
        {
            if(visit_status_dir[i]==false && visit_status_rev_dir[i]==false)
            {
                return false;
            }
        }
        return true;
    }
    void undir_connected_components()
    {
        Traverse_Choice('D');
    }
    bool bipartite_check()
    {
        reset();
        queue<int> temp;
        for(int i=0; i<num_of_vertices&&bipart[i]==WHITE; i++)
        {
            temp.push(i);
            bipart[i]=RED;
            while(!temp.empty())
            {
                int root=temp.front();
                temp.pop();
                for(vector<dest_weight>::iterator it=dir_graph[root].begin(); it!=dir_graph[root].end(); it++)
                {
                    if(bipart[(*it).first]==bipart[root])
                    {
                        return false;
                    }
                    if(bipart[(*it).first]==WHITE)
                    {
                        if(bipart[root]==RED)
                        {
                            bipart[(*it).first]=BLUE;
                        }
                        else
                        {
                            bipart[(*it).first]=RED;
                        }
                        temp.push((*it).first);
                    }
                }
            }
            return true;
        }
    }
    int find_set(int v)
    {
        if(v==sources[v])
        {
            return v;
        }
        else
        {
            return find_set(sources[v]);
        }
    }
    void union_set(int u,int v)
    {
        sources[u]=sources[v];
    }
    void sort_by_edges()//Order of E(square)
    {
        for(int i=0; i<num_of_edges; i++)
        {
            for(int j=i+1; j<num_of_edges; j++)
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
        for(int i=0; i<num_of_edges; i++)
        {
            for(int j=i+1; j<num_of_edges; j++)
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
    void mst_kruskal()//Order of Elog(E) if sort stl used.
    //As my manual sort takes E(square) so the algo becomes of order E(square)
    {
        int i,temp_u,temp_v;
        sort_by_edges();
        for(int i=0; i<List_of_Edges.size(); i++)
        {
            temp_u=find_set(List_of_Edges[i].first.first);
            temp_v=find_set(List_of_Edges[i].first.second);
            if(temp_u!=temp_v)
            {
                Kruskal_MST_Edges.push_back(List_of_Edges[i]);
                union_set(temp_u,temp_v);
                cost_k+=List_of_Edges[i].second;
            }
        }
    }
    void mst_prims()//Order of Elog(V)
    {
        priority_queue <pair<double,int>,vector<pair<double,int>>,greater<pair<double,int>>> min_heap;
        int prim_src=0;
        min_heap.push(make_pair(0,prim_src));
        key[prim_src]=0;
        while(!min_heap.empty())
        {
            int temp_u=min_heap.top().second;
            min_heap.pop();
            if(mst_status[temp_u]==true)
            {
                continue;
            }
            mst_status[temp_u]=true;

            for(vector<dest_weight>::iterator it=dir_graph[temp_u].begin(); it!=dir_graph[temp_u].end(); it++)
            {
                int temp_v=(*it).first;
                double temp_w=(*it).second;
                if(mst_status[temp_v]==false && key[temp_v]>temp_w)
                {
                    key[temp_v]=temp_w;
                    min_heap.push(make_pair(key[temp_v],temp_v));
                    parent[temp_v]=temp_u;
                }

            }

        }
        for(int i=0; i<num_of_vertices; i++)
        {
            cost_p+=key[i];
        }

    }
    void Shortest_path_Djakstra(int source)//Complexity Elog(V)
    {
        reset();
        priority_queue <pair<double,int>,vector<pair<double,int>>,greater<pair<double,int>>> pq;
        pq.push(make_pair(0,source));
        bfs_src_distances[source]=0;
        while(!pq.empty())
        {
            int temp=pq.top().second;
            pq.pop();
            if(visit_status_dir[temp]==false)
            {
                visit_status_dir[temp]=true;
                for(int i=0; i<dir_graph[temp].size(); i++)
                {
                    int temp_dest=dir_graph[temp][i].first;
                    if(visit_status_dir[temp_dest]==false)
                    {
                        double dist_1=bfs_src_distances[temp]+dir_graph[temp][i].second;
                        double dist_2=bfs_src_distances[temp_dest];
                        if(dist_2>dist_1)
                        {
                            pq.push(make_pair(dist_1,temp_dest));
                            bfs_src_distances[temp_dest]=dist_1;
                            bfs_parent[temp_dest]=temp;
                        }
                    }


                }
            }
        }


    }
    void Shortest_path_Least_Edges(int source)//Complexity Elog(V)
    {
        reset();
        priority_queue <pair<double,int>,vector<pair<double,int>>,greater<pair<double,int>>> pq;
        pq.push(make_pair(0,source));
        for(int i=0; i<num_of_vertices; i++)
        {
            bfs_visit_count[i]=0;
        }
        bfs_src_distances[source]=0;
        while(!pq.empty())
        {
            int temp=pq.top().second;
            pq.pop();
            if(visit_status_dir[temp]==false)
            {
                visit_status_dir[temp]=true;
                for(int i=0; i<dir_graph[temp].size(); i++)
                {
                    int temp_dest=dir_graph[temp][i].first;
                    if(visit_status_dir[temp_dest]==false)
                    {
                        double dist_1=bfs_src_distances[temp]+dir_graph[temp][i].second;
                        double dist_2=bfs_src_distances[temp_dest];
                        if(dist_2>dist_1)
                        {
                            pq.push(make_pair(dist_1,temp_dest));
                            bfs_src_distances[temp_dest]=dist_1;
                            bfs_parent[temp_dest]=temp;
                            bfs_visit_count[temp_dest]=bfs_visit_count[temp]+1;
                        }
                        if((dist_2==dist_1)&&(bfs_visit_count[temp]+1<bfs_visit_count[temp_dest]))
                        {
                            bfs_parent[temp_dest]=temp;
                            bfs_visit_count[temp_dest]=bfs_visit_count[temp]+1;
                        }
                    }


                }
            }
        }


    }
    void Shortest_path_Generalized(int source,vector<double> vert_cost)
    {
        reset();
        priority_queue <pair<double,int>,vector<pair<double,int>>,greater<pair<double,int>>> pq;
        bfs_src_distances[source]=vert_cost[source];
        pq.push(make_pair(bfs_src_distances[source],source));
        while(!pq.empty())
        {
            int temp=pq.top().second;
            pq.pop();
            if(visit_status_dir[temp]==false)
            {
                visit_status_dir[temp]=true;
                for(int i=0; i<dir_graph[temp].size(); i++)
                {
                    int temp_dest=dir_graph[temp][i].first;
                    if(visit_status_dir[temp_dest]==false)
                    {
                        double dist_1=bfs_src_distances[temp]+dir_graph[temp][i].second+vert_cost[temp_dest];
                        double dist_2=bfs_src_distances[temp_dest];
                        if(dist_2>dist_1)
                        {
                            pq.push(make_pair(dist_1,temp_dest));
                            bfs_src_distances[temp_dest]=dist_1;
                            bfs_parent[temp_dest]=temp;
                        }
                    }


                }
            }
        }


    }
    bool Shortest_path_BellmanFord(int source)//Complexity VE
    {
        reset();
        for(int i=0; i<num_of_vertices; i++)
        {
            bfs_src_distances[i]=inf;
        }
        bfs_src_distances[source]=0;
        for(int i=0; i<num_of_vertices-1; i++)
        {
            for(int j=0; j<num_of_edges; j++)
            {
                int u=List_of_Edges[j].first.first;
                int v=List_of_Edges[j].first.second;
                double w=List_of_Edges[j].second;
                if(bfs_src_distances[u]+w<bfs_src_distances[v])
                {
                    bfs_src_distances[v]=bfs_src_distances[u]+w;
                    bfs_parent[v]=u;
                }
            }
        }
        for(int j=0; j<num_of_edges; j++)
        {
            int u=List_of_Edges[j].first.first;
            int v=List_of_Edges[j].first.second;
            double w=List_of_Edges[j].second;
            if(bfs_src_distances[u]+w<bfs_src_distances[v])
            {
                cout<<"The graph contains a negative cycle";
                return false;
            }
        }
        cout<<"The graph does not contain a negative cycle";
        return true;
    }
    bool Currency_Trading_BellmanFord(int source)//Complexity VE
    {
        reset();
        for(int i=0; i<num_of_vertices; i++)
        {
            bfs_src_distances[i]=inf;
        }
        bfs_src_distances[source]=0;
        for(int i=0; i<num_of_vertices-1; i++)
        {
            for(int j=0; j<num_of_edges; j++)
            {
                int u=List_of_Edges[j].first.first;
                int v=List_of_Edges[j].first.second;
                double w=List_of_Edges[j].second;
                if(bfs_src_distances[u]+w<bfs_src_distances[v])
                {
                    bfs_src_distances[v]=bfs_src_distances[u]+w;
                    bfs_parent[v]=u;
                }
            }
        }
        for(int j=0; j<num_of_edges; j++)
        {
            int u=List_of_Edges[j].first.first;
            int v=List_of_Edges[j].first.second;
            double w=List_of_Edges[j].second;
            if(bfs_src_distances[u]+w<bfs_src_distances[v])
            {
                cout<<"The graph contains a negative cycle";
                return false;
            }
        }
        cout<<"The graph does not contain a negative cycle";
        return true;
    }
    bool Longest_path_BellmanFord(int source)//Complexity VE
    {
        reset();
        bfs_src_distances[source]=0;
        for(int i=0; i<num_of_vertices-1; i++)
        {
            for(int j=0; j<num_of_edges; j++)
            {
                int u=List_of_Edges[j].first.first;
                int v=List_of_Edges[j].first.second;
                double w=-List_of_Edges[j].second;
                if(bfs_src_distances[u]!=INT_MAX && bfs_src_distances[u]+w<bfs_src_distances[v])
                {
                    bfs_src_distances[v]=bfs_src_distances[u]+w;
                    bfs_parent[v]=u;
                }
            }
        }
        for(int j=0; j<num_of_edges; j++)
        {
            int u=List_of_Edges[j].first.first;
            int v=List_of_Edges[j].first.second;
            double w=-List_of_Edges[j].second;
            if(bfs_src_distances[u]!=INT_MAX && bfs_src_distances[u]+w<bfs_src_distances[v])
            {
                cout<<"The graph contains a negative cycle";
                return false;
            }
        }
        cout<<"The graph does not contain a negative cycle";
        return true;
    }

    void trace_path_rec(int dest)
    {
        if(bfs_parent[dest]==-1)
        {
            cout<<dest;
            return;
        }
        trace_path_rec(bfs_parent[dest]);
        cout<<"->"<<dest;
    }
    void print_path(int source,int dest)
    {
        cout<<"Shortest Path Cost:"<<bfs_src_distances[dest]<<endl;
        cout<<"Number of Edges: "<<bfs_visit_count[dest]<<endl;
        cout<<"Path: ";
        trace_path_rec(dest);
        cout<<endl;

    }
    void All_Pair_Shortest_Path_Floyd_Warshall()
    {
        Result_Mat= new double*[num_of_vertices];
        for(int i=0; i<num_of_vertices; i++)
        {
            Result_Mat[i]=new double[num_of_vertices];
        }
        for(int i=0; i<num_of_vertices; i++)
        {
            for(int j=0; j<num_of_vertices; j++)
            {
                Result_Mat[i][j]=Adj_Matt[i][j];
            }
        }

        for(int k=0; k<num_of_vertices; k++)
        {
            for(int i=0; i<num_of_vertices; i++)
            {
                for(int j=0; j<num_of_vertices; j++)
                {
                    if(Result_Mat[i][k]+Result_Mat[k][j]<Result_Mat[i][j])
                    {
                        Result_Mat[i][j]=Result_Mat[i][k]+Result_Mat[k][j];
                        Predecessor[i][j]=Predecessor[i][k];
                    }
                }
            }
        }
    }
    double ** Shortest_Path_Extention(double **mat_A,double **mat_B)
    {
        double **Temp_Mat= new double*[num_of_vertices];
        for(int i=0; i<num_of_vertices; i++)
        {
            Temp_Mat[i]=new double[num_of_vertices];
        }
        for(int i=0; i<num_of_vertices; i++)
        {
            for(int j=0; j<num_of_vertices; j++)
            {
                Temp_Mat[i][j]=INT_MAX;
                for(int k=0; k<num_of_vertices; k++)
                {
                    if(mat_A[i][k]+mat_B[k][j]<Temp_Mat[i][j])
                    {
                        Temp_Mat[i][j]=mat_A[i][k]+mat_B[k][j];
                    }
                }
            }

        }
        return Temp_Mat;
    }
    void All_Pair_Shortest_Path_Matrix_Multiplication()
    {
        Result_Mat= new double*[num_of_vertices];
        for(int i=0; i<num_of_vertices; i++)
        {
            Result_Mat[i]=new double[num_of_vertices];
        }
        for(int i=0; i<num_of_vertices; i++)
        {
            for(int j=0; j<num_of_vertices; j++)
            {
                Result_Mat[i][j]=Adj_Matt[i][j];
            }
        }
        for(int i=1; i<num_of_vertices; i++)
        {
            Result_Mat=Shortest_Path_Extention(Result_Mat,Adj_Matt);
        }
    }
    void All_Pair_Shortest_Path_Matrix_Multiplication_Faster()
    {
        Result_Mat= new double*[num_of_vertices];
        for(int i=0; i<num_of_vertices; i++)
        {
            Result_Mat[i]=new double[num_of_vertices];
        }
        for(int i=0; i<num_of_vertices; i++)
        {
            for(int j=0; j<num_of_vertices; j++)
            {
                Result_Mat[i][j]=Adj_Matt[i][j];
            }
        }
        int m=1;
        while(m<num_of_vertices-1)
        {
            Result_Mat=Shortest_Path_Extention(Result_Mat,Result_Mat);
            m=2*m;
        }
    }

    int bfs(double ** rGraph, int s, int t, int parent[])
{
    // Create a visited array and mark all vertices as not visited
    bool visited[num_of_vertices];
    memset(visited, 0, sizeof(visited));

    // Create a queue, enqueue source vertex and mark source vertex
    // as visited
    queue <int> q;
    q.push(s);
    visited[s] = true;
    parent[s] = -1;

    // Standard BFS Loop
    while (!q.empty())
    {
        int u = q.front();
        q.pop();

        for (int v=0; v<num_of_vertices; v++)
        {
            if (visited[v]==false && rGraph[u][v] > 0)
            {
                q.push(v);
                parent[v] = u;
                visited[v] = true;
            }
        }
    }

    // If we reached sink in BFS starting from source, then return
    // true, else false
    return (visited[t] == true);
}

// A DFS based function to find all reachable vertices from s.  The function
// marks visited[i] as true if i is reachable from s.  The initial values in
// visited[] must be false. We can also use BFS to find reachable vertices
void dfs(double ** rGraph, int s, bool visited[])
{
    visited[s] = true;
    for (int i = 0; i < num_of_vertices; i++)
       if (rGraph[s][i] && !visited[i])
           dfs(rGraph, i, visited);
}

double max_flow(int s, int t)
{
    int u, v;

    // Create a residual graph and fill the residual graph with
    // given capacities in the original graph as residual capacities
    // in residual graph
    double ** rGraph;// rGraph[i][j] indicates residual capacity of edge i-j
    rGraph=new double*[num_of_vertices];
        for(int i=0; i<num_of_vertices; i++)
        {
            rGraph[i]=new double[num_of_vertices];
        }
    for (u = 0; u < num_of_vertices; u++)
    {
        for (v = 0; v < num_of_vertices; v++)
             {
                 if(Adj_Matt[u][v]==INT_MAX)
                 {
                     rGraph[u][v]=0;
                 }
                 else
                 {
                     rGraph[u][v]=Adj_Matt[u][v];
                 }
             }
    }

    int parent[num_of_vertices];  // This array is filled by BFS and to store path
    double max_flow=0;
    // Augment the flow while there is a path from source to sink
    while (bfs(rGraph, s, t, parent))
    {
        // Find minimum residual capacity of the edges along the
        // path filled by BFS. Or we can say find the maximum flow
        // through the path found.
        double path_flow = INT_MAX;
        for (v=t; v!=s; v=parent[v])
        {
            u = parent[v];
            //path_flow = min(path_flow, rGraph[u][v]);
            if(path_flow>rGraph[u][v])
            {
                path_flow=rGraph[u][v];
            }
        }

        // update residual capacities of the edges and reverse edges
        // along the path
        for (v=t; v != s; v=parent[v])
        {
            u = parent[v];
            rGraph[u][v] -= path_flow;
            rGraph[v][u] += path_flow;
        }
        max_flow+=path_flow;
    }
    for(int i=0; i<num_of_vertices; i++)
        {
            delete[] rGraph[i];
        }
        delete[]rGraph;
    return max_flow;

}
// Prints the minimum s-t cut
void minCut(int s, int t)
{
    int u, v;

    // Create a residual graph and fill the residual graph with
    // given capacities in the original graph as residual capacities
    // in residual graph
      double ** rGraph;// rGraph[i][j] indicates residual capacity of edge i-j
    rGraph=new double*[num_of_vertices];
        for(int i=0; i<num_of_vertices; i++)
        {
            rGraph[i]=new double[num_of_vertices];
        }
    for (u = 0; u < num_of_vertices; u++)
    {
        for (v = 0; v < num_of_vertices; v++)
             {
                 if(Adj_Matt[u][v]==INT_MAX)
                 {
                     rGraph[u][v]=0;
                 }
                 else
                 {
                     rGraph[u][v]=Adj_Matt[u][v];
                 }
             }
    }

    int parent[num_of_vertices];  // This array is filled by BFS and to store path

    // Augment the flow while there is a path from source to sink
    while (bfs(rGraph, s, t, parent))
    {
        // Find minimum residual capacity of the edges along the
        // path filled by BFS. Or we can say find the maximum flow
        // through the path found.
        int path_flow = INT_MAX;
        for (v=t; v!=s; v=parent[v])
        {
            u = parent[v];
            if(path_flow>rGraph[u][v])
            {
                path_flow=rGraph[u][v];
            }
            //path_flow = min(path_flow, rGraph[u][v]);
        }

        // update residual capacities of the edges and reverse edges
        // along the path
        for (v=t; v != s; v=parent[v])
        {
            u = parent[v];
            rGraph[u][v] -= path_flow;
            rGraph[v][u] += path_flow;
        }
    }

    // Flow is maximum now, find vertices reachable from s
    bool visited[num_of_vertices];
    memset(visited, false, sizeof(visited));
    dfs(rGraph, s, visited);

    // Print all edges that are from a reachable vertex to
    // non-reachable vertex in the original graph
    for (int i = 0; i < num_of_vertices; i++)
      for (int j = 0; j < num_of_vertices; j++)
         if (visited[i] && !visited[j] && Adj_Matt[i][j])
              cout << i << " - " << j << endl;
    for(int i=0; i<num_of_vertices; i++)
        {
            delete[] rGraph[i];
        }
        delete[]rGraph;
       return;
}

bool bpm(bool **bpGraph,int M,int N, int u,
         bool seen[], int matchR[])
{
    // Try every job one by one
    for (int v = 0; v < N; v++)
    {
        // If applicant u is interested in
        // job v and v is not visited
        if (bpGraph[u][v] && !seen[v])
        {
            // Mark v as visited
            seen[v] = true;

            // If job 'v' is not assigned to an
            // applicant OR previously assigned
            // applicant for job v (which is matchR[v])
            // has an alternate job available.
            // Since v is marked as visited in
            // the above line, matchR[v] in the following
            // recursive call will not get job 'v' again
            if (matchR[v] < 0 || bpm(bpGraph,M,N, matchR[v],
                                     seen, matchR))
            {
                matchR[v] = u;
                return true;
            }
        }
    }
    return false;
}

// Returns maximum number
// of matching from M to N
int maxBPM(bool **bpGraph,int M,int N)
{
    // An array to keep track of the
    // applicants assigned to jobs.
    // The value of matchR[i] is the
    // applicant number assigned to job i,
    // the value -1 indicates nobody is
    // assigned.
    int matchR[N];

    // Initially all jobs are available
    memset(matchR, -1, sizeof(matchR));

    // Count of jobs assigned to applicants
    int result = 0;
    for (int u = 0; u < M; u++)
    {
        // Mark all jobs as not seen
        // for next applicant.
        bool seen[N];
        memset(seen, 0, sizeof(seen));

        // Find if the applicant 'u' can get a job
        if (bpm(bpGraph,M,N, u, seen, matchR))
            result++;
    }
    return result;
}

    ~Graph()
    {
        for(int i=0; i<num_of_vertices; i++)
        {
            delete[] Result_Mat[i];
            delete[] Adj_Matt[i];
        }
        delete[]Result_Mat;
        delete[]Adj_Matt;
    }
    void Print_Edges(vector<pair<vertice,double>> test)//Order of V
{

    cout<<"Edges"<<"   "<<"Weight"<<endl;
    for(int i=0; i<test.size(); i++)
    {
        cout<<test[i].first.first<<" -- "<<test[i].first.second<<"  "<<test[i].second<<endl;
    }
}
void Print_Edges(vector<int> arr,vector<double> key,int num_of_vertices)//Order of V
{

    cout<<"Edges"<<"   "<<"Weight"<<endl;
    for(int i=1; i<num_of_vertices; i++)
    {
        cout<<arr[i]<<" -- "<<i<<"  "<<key[i]<<endl;

    }
}

void print_edge_Set(vector<int> arr,int num_of_vertices)//Order of V(square)
{
    int temp=0;
    vector<int> dest;
    int cnt=1;
    bool check=false;
    cout<<"List of Edges Selected By Prim's:";
    cout<<"{";
    while(1)
    {
        if(check==false)
        {
            for(int i=1; i<num_of_vertices; i++)
            {
                if(arr[i]==temp)
                {
                    temp=i;
                    if(cnt!=1)
                    {
                        cout<<",";
                    }
                    cout<<"("<<arr[i]<<","<<i<<")";
                    dest.push_back(i);
                    cnt++;
                }
            }
            check=true;
        }
        if(cnt<num_of_vertices-1)
        {
            for(int i=1; i<num_of_vertices; i++)
            {
                bool present=false;
                for(int j=0; j<dest.size(); j++)
                {
                    if(i==dest[j])
                    {
                        present=true;
                        break;
                    }
                }
                if(present==false)
                {
                    if(cnt!=1)
                    {
                        cout<<",";
                    }
                    cout<<"("<<arr[i]<<","<<i<<")";
                    cnt++;
                }
            }
        }
        else
        {
            break;
        }

    }
    cout<<"}"<<endl;
}


void print_edge_Set(vector<pair<vertice,double>> test)//Order of V
{

    cout<<"List of Edges Selected By Kruskal's:";
    cout<<"{";
    for(int i=0; i<test.size(); i++)
    {
        if(i!=0)
        {
            cout<<",";
        }
        cout<<"("<<test[i].first.first<<","<<test[i].first.second<<")";
    }
    cout<<"}"<<endl;
}
void print_APSP(double **mat,int num_of_vertices)
{
    cout<<"Shortest Distance Mattrix:"<<endl;
    for(int i=0; i<num_of_vertices; i++)
    {
        for(int j=0; j<num_of_vertices; j++)
        {
            if(mat[i][j]==INT_MAX)
            {
                cout<<"INF  ";
            }
            else
            {
                cout<<mat[i][j]<<"  ";
            }
        }
        cout<<endl;
    }

}
vector<int> Construct_Path_APSP(int startv,int endv,int ** pred)
{
    startv=startv-1;
    endv=endv-1;
    if (pred[startv][endv] == -1)
        return {};

    // Storing the path in a vector
    vector<int> path;
    path.push_back(startv);
    while (startv != endv)
    {
        startv = pred[startv][endv];
        path.push_back(startv);
    }
    return path;
}
void print_path_APSP(vector<int> path)
{
    int n=path.size();
    for(int i=0; i<n-1; i++)
    {
        cout<<path[i]+1<<"  ->  ";
    }
    cout<<path[n-1]+1;
}

edge heaviest_edge()
{
    int u,v;
    double max=0;
    for(int i=0;i<num_of_vertices;i++)
    {
        for (int j=0;j<num_of_vertices;j++)
        {
            if(Adj_Matt[i][j]!=INT_MAX && Adj_Matt[i][j]!=0 && Adj_Matt[i][j]>max)
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
    int u,v;
    double max=INT_MAX;
    for(int i=0;i<num_of_vertices;i++)
    {
        for (int j=0;j<num_of_vertices;j++)
        {
            if(Adj_Matt[i][j]!=INT_MAX && Adj_Matt[i][j]!=0 && Adj_Matt[i][j]<max)
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

    double weight_factor =  min_weight + alpha*(max_weight-min_weight);

    vector<edge> RCL_edge ;

     for(int i=0;i<num_of_vertices;i++)
    {
        for (int j=0;j<num_of_vertices;j++)
        {
            if(Adj_Matt[i][j]!=INT_MAX && Adj_Matt[i][j]!=0 && Adj_Matt[i][j]>weight_factor)
            {
              RCL_edge.push_back(make_pair(make_pair(i,j),Adj_Matt[i][j]));
            }
        }
    }
    edge selected_edge = RCL_edge[rand()%RCL_edge.size()];
    set<int> final_x;
    set<int> final_y;
    

    final_x.insert(selected_edge.first.first);
    final_y.insert(selected_edge.first.second);
    final_weight+=selected_edge.second;

    

    while(true)
    {
        set<int> union_xy;
        set_union(final_x.begin(),final_x.end(),final_y.begin(),final_y.end(),inserter(union_xy,union_xy.end()));
        if(all_veritces == union_xy)
        {
            break;
        }
        set<int> remaining_vertices;
        set_difference(all_veritces.begin(),all_veritces.end(),union_xy.begin(),union_xy.end(),inserter(remaining_vertices,remaining_vertices.end()));

        vector<double> sigma_x;
        vector<double> sigma_y;
        vector<int> sigma_vertices;

        for(auto i: remaining_vertices)
        {
            double temp_x=0;
            double temp_y=0;
            for(auto j : final_x)
            {
                 if(Adj_Matt[i][j]!=INT_MAX )
            {
                temp_x += Adj_Matt[i][j];
            } 

            }
            for(auto j : final_y)
            {
                 if(Adj_Matt[i][j]!=INT_MAX )
            {
                temp_y += Adj_Matt[i][j];
            } 
            
            }
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

        vector<int> RCL_vertices;
        for(int i=0;i<remaining_vertices.size();i++)
        {
            if(max(sigma_x[i],sigma_y[i])>weight_factor)
            {
                RCL_vertices.push_back(sigma_vertices[i]);
            }
        }
        int selected_vertice = RCL_vertices[rand()%RCL_vertices.size()];
        int selected_indx;
        for(int i=0;i<sigma_vertices.size();i++)
        {
            if(selected_vertice==sigma_vertices[i])
            {
                selected_indx=i;
                break;
            }
        }
        if(sigma_x[selected_indx]>=sigma_y[selected_indx])
        {
            final_x.insert(selected_indx);
            final_weight+=sigma_x[selected_indx];
        }
        else
        {
            final_y.insert(selected_indx);
            final_weight += sigma_y[selected_indx];
        }

    }
    final.first.first=final_x;
    final.first.second=final_y;
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
                for(auto j : final.first.first)
                {
                    if(Adj_Matt[i][j]!=INT_MAX && Adj_Matt[i][j]!=0)
                    {
                        sigma_s+=Adj_Matt[i][j];
                    }
                }
                for(auto j : final.first.second)
                {
                    if(Adj_Matt[i][j]!=INT_MAX && Adj_Matt[i][j]!=0)
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
            if(Adj_Matt[i][j]!=INT_MAX && Adj_Matt[i][j]!=0)
            {
                final_weight+=Adj_Matt[i][j];
            }
        }
    }
    final.second=final_weight;
    return final;
}
cut grasp_maxcut()
{
    double max_weight = -INT_MAX;
    cut final_cut;
    double prev_weight,after_weight;
    while(true)
    {
        cut temp_cut =  semi_greedy_maxcut();
        prev_weight=temp_cut.second;
        temp_cut = local_search_maxcut(temp_cut);
        after_weight=temp_cut.second;
        if(temp_cut.second>max_weight)
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
};



int main()
{
    
        int num_v,num_edge;
        cin>>num_v>>num_edge;
        Graph X(num_v,num_edge);
       for(int i=0;i<num_edge;i++)
       {   int u,v;
           cin>>u>>v;
           X.Add_Edge_Directed(u,v);
       }
       X.print_AdjM();
    return 0;
}
