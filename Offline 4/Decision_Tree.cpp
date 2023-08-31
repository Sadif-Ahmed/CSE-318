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
using namespace std;
void print_datatable(string **temp,int num_examples,int num_attributes)
{
    cout<<"The Data Table:"<<endl;
    for(int i=0;i<=num_examples;i++)
    {
        for(int j=0;j<=num_attributes+1;j++)
        {
            cout<<temp[i][j]<<"   ";
        }
        cout<<endl;
    }
}
class Node {
	public:
		int criteria_attr_indx;
		string attr_val;

		int tree_index;
		bool is_leaf;
		string label;

		vector<int > children;

		Node() {
			is_leaf = false;
		}
};
class Decision_Tree
{
    public:
    string **datatable;
    vector<string> attr_names;
    vector<vector<string>> attr_values;
    int data_count;
    Decision_Tree(string **datatable,vector<string> attr_names,vector<vector<string>>attr_values,int data_count)
    {
        this->datatable=datatable;
        this->data_count=data_count;
        this->attr_names=attr_names;
        this->attr_values=attr_values;
    }
};
int main()
{
    fstream infile("car evaluation dataset/car.data",std::ios_base::in);
    int num_of_examples=50;
    int num_of_attributes=6;
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
        cout<<datastream<<endl;
        stringstream stream(datastream);
        int attr_count=0;
        datatable[j][attr_count++]=to_string(j);
        while(getline(stream,temp,','))
        {
            datatable[j][attr_count++]=temp;   
        }
    }
    print_datatable(datatable,num_of_examples,num_of_attributes);

    string **traintable,**testtable;
    int total_train=num_of_examples*0.8;
    int total_test=num_of_examples*0.2;
    traintable = new string*[total_train];
    testtable = new string*[total_test];
    int num_train=0;
    int num_test=0;

    for(int i=0;i<total_train;i++)
    {
        traintable[i] = new string[num_of_attributes+1];
    }
    
    for(int i=0;i<total_test;i++)
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
    cout<<num_test<<"   "<<num_train<<endl;
    //print_datatable(traintable,num_train-1,num_of_attributes-2);
    //print_datatable(testtable,num_test-1,num_of_attributes-2);
    Decision_Tree tree(traintable,attr_names,attr_values,num_train);
    cout<<"Tree Training Set:"<<endl;
    print_datatable(tree.datatable,tree.data_count-1,tree.attr_names.size()-2);
    return 0;
}