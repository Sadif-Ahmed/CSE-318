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
		string attr_val;

		int tree_index;
		bool is_leaf;
		string label;

		vector<int > children;

		Node(int index) {
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

        Node root(0);
        add_child(root);
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

			map<string, vector<int> > attr_val_map;
			for(int i=0;i<table.size();i++) {
				attr_val_map[table[i][attr_index]].push_back(i);
			}

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
				decision_count_store[table[i].back()]++;
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

			map<string, vector<int> > attr_val_map;
			for(int i=0;i<table.size();i++) {
				attr_val_map[table[i][attr_index]].push_back(i);
			}

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
            if(table[i].back()!=table[i+1].back())
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
int main()
{
    fstream infile("car evaluation dataset/car.data",std::ios_base::in);
    int num_of_examples=10;
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
    //cout<<tree.split_attr_calc(tree.datatable,0)<<endl;
    return 0;
}