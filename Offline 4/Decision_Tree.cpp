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
#include<sstream>
#include <cstdlib>
#include <random>
using namespace std;
class Node {
	public:
		int criteria_attr_indx;
		string attr;

		int tree_index;
		bool is_leaf;
		string label;

		vector<int > children;

		Node(int index) {
			is_leaf = false;
            tree_index=index;
		}
        Node(string attr,int index) {
			this->attr=attr;
            is_leaf = false;
            tree_index=index;
		}
};
class Decision_Tree
{
    public:
    vector<vector<string>> datatable;
    vector<string> attr_names;
    vector<vector<string>> attr_values;
    int data_count;
    vector<Node> tree;
    int node_count=0;

    Decision_Tree(string **datatable,vector<string> attr_names,vector<vector<string>>attr_values,int data_count)
    {
        this->data_count=data_count;
        this->attr_names=attr_names;
        this->attr_values=attr_values;
        for(int i=0;i<data_count;i++)
        {
            vector<string> temp;
            for(int j=0;j<=attr_names.size();j++)
            {
                temp.push_back(datatable[i][j]);
            }
            this->datatable.push_back(temp);
        }
    }
    void print_data()
    {
        cout<<"Tree Training Set:"<<endl;
        for(int i=0;i<attr_names.size();i++)
        {
            cout<<attr_names[i]<<"\t";
        }
        cout<<endl;
        for(int i=0;i<datatable.size();i++)
        {
            for(int j=0;j<datatable[i].size();j++)
            {
                cout<<datatable[i][j]<<"\t";
            }
            cout<<endl;
        }
    }
    void add_child(Node temp)
    {
        tree.push_back(temp);
    }
    void update_table(vector<vector<string>> newtable)
    {
        this->datatable=newtable;
    }
    double split_attr_calc(vector<vector<string>> table, int attr_index) {
			double ret = 0.0;

			int data_count = table.size();

			map<string, vector<int> > attr_val_map=attribute_value_map(table,attr_index);
			

			for(auto iter=attr_val_map.begin(); iter != attr_val_map.end(); iter++) 
            {
                int next_count = iter->second.size();
				double d = (double)next_count/data_count;
				ret += -1.0 * d * log(d) / log(2);
			}

			return ret;
		}
		double get_decision_info(vector<vector<string>> table) 
        {
			double ret = 0.0;

			int data_count = table.size();
			map<string, int> decision_count_store;

			for(int i=0;i<table.size();i++) {
				decision_count_store[table[i][attr_names.size()-1]]++;
			}

			for(auto iter=decision_count_store.begin(); iter != decision_count_store.end(); iter++) {
				double p = (double)iter->second/data_count;

				ret += -1.0 * p * log(p)/log(2);
			}

			return ret;
		}
    double info_attr(vector<vector<string>> table, int attr_index) {
			double ret = 0.0;

			int data_count = table.size();

			map<string, vector<int> > attr_val_map=attribute_value_map(table,attr_index);
			

			for(auto iter=attr_val_map.begin(); iter != attr_val_map.end(); iter++) {
				vector<vector<string>> new_table;
				for(int i=0;i<iter->second.size(); i++) {
					new_table.push_back(table[iter->second[i]]);
				}
				// int next_count = (int)new_table.size();
                int next_count = iter->second.size();
				ret += (double)next_count/data_count * get_decision_info(new_table);
			}

			return ret;
		}
    double gain(vector<vector<string>> table, int attr_index) {
                return get_decision_info(table)-info_attr(table, attr_index);
            }        
    double gain_ratio(vector<vector<string>> table, int attr_index) {
                return gain(table, attr_index)/split_attr_calc(table, attr_index);
            }   
    bool check_leaf(vector<vector<string>> table)
    {
        for(int i=0;i<table.size()-1;i++)
        {
            if(table[i][attr_names.size()-1]!=table[i+1][attr_names.size()-1])
            {
                return false;
            }
        }
        return true;
    }   
    int choose_attr(vector<vector<string>> table)
    {
        int choosen_attr =-1;
        double max_gain_ratio=0.0;

        for(int i=0;i<attr_names.size()-1;i++)
        {
            double temp_gain_ratio=gain_ratio(table,i);
            if(temp_gain_ratio>max_gain_ratio)
            {
                max_gain_ratio=temp_gain_ratio;
                choosen_attr=i;
            }
        }
        return choosen_attr;
    }  
    pair<string,int> majority_label(vector<vector<string>> table)
    {
        string major_label;
        int major_count=0;
        map <string,int> label_count;
        for(int i=0;i<table.size();i++)
        {
            label_count[table[i][attr_names.size()-1]]++;
        }
        map<string, int>::iterator iter;
        for(iter=label_count.begin();iter!=label_count.end();iter++)
        {
            if(iter->second>major_count)
            {
                major_label=iter->first;
                major_count=iter->second;
            } 
        }
        return make_pair(major_label,major_count);
    }
    void rec_generate_tree(vector<vector<string>> table,int node_index)
    {
        if(check_leaf(table))
        {
            tree[node_index].is_leaf=true;
            tree[node_index].label=table[table.size()-1][attr_names.size()-1];
            return;
        }

        int selected_attr = choose_attr(table);


		
        tree[node_index].criteria_attr_indx = selected_attr;

        pair<string,int> plurality_label = majority_label(table);
        if(plurality_label.second>=table.size()*0.8)
        {
            tree[node_index].is_leaf=true;
            tree[node_index].label=plurality_label.first;
            return;
        } 

        map<string, vector<int> > attr_val_map=attribute_value_map(table,selected_attr); 
        
        for(int i=0;i<attr_values[selected_attr].size();i++)
        {
            string attr = attr_values[selected_attr][i];

            vector<vector<string>> candidatetable;

            vector<int> candidates = attr_val_map[attr];

            for(int i=0;i<candidates.size();i++)
            {
                candidatetable.push_back(table[candidates[i]]);
            }

            Node child_node(attr,tree.size());
            tree[node_index].children.push_back(child_node.tree_index);
            add_child(child_node);
            node_count++;

            if(candidatetable.size()==0)
            {
                child_node.is_leaf=true;
                child_node.label= majority_label(table).first;
                tree[child_node.tree_index] = child_node;
            }
            else
            {
                rec_generate_tree(candidatetable,child_node.tree_index);
            }

        }  
    } 
    void generate_tree()
    {
        node_count++;
        Node root(0);
        add_child(root);
        rec_generate_tree(datatable,0);
    }
    map<string,vector<int>> attribute_value_map(vector<vector<string>> table,int selected_attribute)
    {
        map<string,vector<int>> attr_val_map;
        for(int i=0;i<table.size();i++) 
        {
				attr_val_map[table[i][selected_attribute]].push_back(i);
		}
        return attr_val_map;

    }       
};
void randomise(string **data,int data_count,int attr_count)
{
    vector<vector<string>> vector_data;
    for(int i=1;i<=data_count;i++)
        {
            vector<string> temp;
            for(int j=1;j<=attr_count;j++)
            {
                temp.push_back(data[i][j]);
            }
            vector_data.push_back(temp);
        }
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(vector_data.begin(), vector_data.end(), g);
    for(int i=0;i<vector_data.size();i++)
    {   
        for(int j=0;j<vector_data[i].size();j++)
        {
            data[i+1][j+1]=vector_data[i][j];
        }
    }
}
void print_datatable(vector<vector<string>> vector_data)
{
    cout<<"Data Table"<<endl;
    for(int i=0;i<vector_data.size();i++)
    {
        for(int j=0;j<vector_data[i].size();j++)
        {
            cout<<vector_data[i][j]<<"  ";
        }
        cout<<endl;
    }
}
void print_datatable(string **temp,int num_examples,int num_attributes)
{
    cout<<"The Data Table:"<<endl;
    for(int i=0;i<=num_examples;i++)
    {
        for(int j=0;j<=num_attributes;j++)
        {
            cout<<temp[i][j]<<"   ";
        }
        cout<<endl;
    }
}
void print_atrr_map(map<string,vector<int>> temp)
{
    map<string,vector<int>>::iterator itr;
    for(itr=temp.begin();itr!=temp.end();itr++)
    {
        cout<<itr->first<<endl;
        for(int i=0;i<itr->second.size();i++)
        {
            cout<<itr->second[i]<<"\t";
        }
        cout<<endl;
    }
}
class Testing
{
    public:
    Decision_Tree *tree;
    vector<vector<string>> testtable;
    vector<string> attr_names;
Testing(Decision_Tree *tree, string **datatable,vector<string> attr_names,int data_count)
{
        this->tree=tree;
        this->attr_names=attr_names;
        for(int i=0;i<data_count;i++)
        {
            vector<string> temp;
            for(int j=0;j<=attr_names.size();j++)
            {
                temp.push_back(datatable[i][j]);
            }
            testtable.push_back(temp);
        }

}

int search(Decision_Tree tree,vector<string> attributes,int tree_index)
{
    if(tree.tree[tree_index].is_leaf)
    {
        return tree_index;
    }

    int criteria_attribute_index = tree.tree[tree_index].criteria_attr_indx;
    for(int i=0;i<tree.tree[tree_index].children.size();i++)
    {
        int child_index = tree.tree[tree_index].children[i];

        if(attributes[criteria_attribute_index] == tree.tree[child_index].attr)
        {
            return search(tree,attributes,child_index);
        }
    }
    return -1;
}
string generate_decision(Decision_Tree tree,vector<string>attributes)
{
    string decision;
    int leaf_index = search(tree,attributes,0);
    if(leaf_index == -1)
    {
        return "Something Went Wrong";
    }
    decision = tree.tree[leaf_index].label;
    return decision;
}
void print_data()
{
        cout<<"Tree Testing Set:"<<endl;
        for(int i=0;i<attr_names.size();i++)
        {
            cout<<attr_names[i]<<"\t";
        }
        cout<<endl;
        for(int i=0;i<testtable.size();i++)
        {
            for(int j=0;j<testtable[i].size();j++)
            {
                cout<<testtable[i][j]<<"\t";
            }
            cout<<endl;
        }
}
void print_solo_data(int test_index)
{
        cout<<"Tree Testing Set(Test Example "<<test_index<<" ):"<<endl;
        for(int i=0;i<attr_names.size();i++)
        {
            cout<<attr_names[i]<<"\t";
        }
        cout<<endl;
        
        for(int j=0;j<testtable[test_index].size();j++)
        {
                cout<<testtable[test_index][j]<<"\t";
        }
        cout<<endl;    
}
};


int main()
{
    fstream infile("car evaluation dataset/car.data",std::ios_base::in);
    int num_of_examples=1738;
    int num_of_attributes=7;
    string **datatable = new string*[num_of_examples+1];
    vector<string> attr_names;
    vector<vector<string>> attr_values;
    for(int i=0;i<=num_of_examples;i++)
    {
        datatable[i] = new string[num_of_attributes+2];
    }

    vector<string> val_types1{"vhigh","high","med","low"};
    vector<string> val_types2{"2","3","4","5more"};
    vector<string> val_types3{"2","4","more"};
    vector<string> val_types4{"small","med","big"};
    vector<string> val_types5{"low","med","high"};
    vector<string> val_types6{"unacc", "acc", "good", "vgood"};
    
    attr_names.push_back("buying");
    attr_values.push_back(val_types1);
    attr_names.push_back("maint");
    attr_values.push_back(val_types1);
    attr_names.push_back("doors");
    attr_values.push_back(val_types2);
    attr_names.push_back("persons");
    attr_values.push_back(val_types3);
    attr_names.push_back("lug_boot");
    attr_values.push_back(val_types4);
    attr_names.push_back("safety");
    attr_values.push_back(val_types5);
    attr_names.push_back("Decision");
    attr_values.push_back(val_types6);


    datatable[0][0]="Example";
    datatable[0][1]="buying";
    datatable[0][2]="maint";
    datatable[0][3]="doors";
    datatable[0][4]="persons";
    datatable[0][5]="lug_boot";
    datatable[0][6]="safety";
    datatable[0][7]="Decision";
    
    //print_datatable(datatable,num_of_examples,num_of_attributes);
    for(int j=1;j<=num_of_examples;j++)
    {
        
        string datastream,temp;
        infile>>datastream;
        //cout<<datastream<<endl;
        stringstream stream(datastream);
        int attr_count=0;
        datatable[j][attr_count++]=to_string(j);
        while(getline(stream,temp,','))
        {
            datatable[j][attr_count++]=temp;   
        }
    }

    //print_datatable(datatable,num_of_examples,num_of_attributes);
    randomise(datatable,num_of_examples,num_of_attributes);
    //print_datatable(datatable,num_of_examples,num_of_attributes);
    
    string **traintable,**testtable;
    traintable = new string*[num_of_examples];
    testtable = new string*[num_of_examples];
    int num_train=0;
    int num_test=0;

    for(int i=0;i<num_of_examples;i++)
    {
        traintable[i] = new string[num_of_attributes+1];
    }
    
    for(int i=0;i<num_of_examples;i++)
    {
        testtable[i] = new string[num_of_attributes+1];
    }

    for(int i=1;i<=num_of_examples;i++)
    {
        for(int j=1;j<=num_of_attributes+1;j++)
        {
            if((i-1)%10 == 2 || (i-1)%10 == 3 )
            {
                testtable[num_test][j-1]=datatable[i][j];
            }
            else
            {
                traintable[num_train][j-1]=datatable[i][j];
            }
        }
        if((i-1)%10 == 2 || (i-1)%10 == 3 )
        {
                num_test++;
        }
        else
        {
                num_train++;
        }
    }
    //cout<<num_test<<"   "<<num_train<<endl;
    //print_datatable(traintable,num_train-1,num_of_attributes-1);
    //print_datatable(testtable,num_test-1,num_of_attributes-1);
    Decision_Tree tree(traintable,attr_names,attr_values,num_train);
    //tree.print_data(); 
    tree.generate_tree();
    cout<<"Tree Node Count: "<<tree.node_count<<endl;
    Testing test(&tree,testtable,attr_names,num_test);
    //test.print_data();
    test.print_solo_data(0);
    string decision = test.generate_decision(tree,test.testtable[0]);
    cout<<"Decision: "<<decision<<endl;
    return 0;
}