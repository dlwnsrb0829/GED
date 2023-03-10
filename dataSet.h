#include<iostream>
#include<sstream>
#include<vector>
#include<fstream>
#include<string>
#include<stack>
#include<cstring>
#include<set>
#include<algorithm>
#include"graph.h"

using namespace std;

class dataSet{
private:
    int v_size;
    int e_size;
    string v_label;
    int * vertex_mapping;
    int * vertex_label_mapping;
    int * edge_label_mapping;
    int * vertex_label_num;
    int * edge_label_num;
    int vertex_set_size;
    int edge_set_size;
    bool is_first_edge;
    int ** vertex_bridge;
    string fileName;
    graph g;
    vector<int> split_vector;
    set<int> vertex_vector;
    set<int> edge_vector;
    vector<int> split(string input, char delimiter, string type);

public:
    dataSet(){};
    dataSet(string fileName){
        v_size = 0;
        e_size = 0;
        v_label = "";
        this->fileName = fileName;
        read_file();
    }
    void read_file();
    graph get_graph();
};

void dataSet :: read_file(){
    ifstream readFile;
    int graph_num = 0;
    readFile.open(this->fileName);
    if(readFile.is_open()){
        char arr[256];
        while(!readFile.eof()){
            readFile.getline(arr, 256);
            switch (arr[0]){
                case 't':
                    split_vector = split(arr, ' ', "graph_num");
                    graph_num = split_vector[0];
                    break;
                case 'v':
                    v_size++;
                    split_vector = split(arr, ' ', "data");
                    v_label += to_string(split_vector[1]) + " ";
                    vertex_vector.insert(split_vector[1]);
                    break;
                case 'e':
                    e_size++;
                    split_vector = split(arr, ' ', "data");
                    edge_vector.insert(split_vector[2]);
                    break;
                default:
                    break;
            }
        }
    }
    readFile.close();
    
    g = graph(v_size, e_size);
    g.set_graph_num(graph_num);
    split_vector = split(v_label, ' ', "");
    vertex_mapping = new int[v_size];
    for(int i = 0 ; i < v_size ; i++){
        vertex_mapping[i] = split_vector[i];
    }
    g.set_v_labels(vertex_mapping);

    vertex_set_size = vertex_vector.size();
    edge_set_size = edge_vector.size();
    vertex_label_mapping = new int[vertex_vector.size()];
    vertex_label_num = new int[vertex_vector.size()];
    edge_label_mapping = new int[edge_vector.size()];
    edge_label_num = new int[edge_vector.size()];
    memset(vertex_label_num, 0, sizeof(int) * vertex_vector.size());
    memset(edge_label_num, 0, sizeof(int) * edge_vector.size());
    vector<int> v1;
    copy(vertex_vector.begin(), vertex_vector.end(), back_inserter(v1));
    for(int i = 0 ; i < vertex_vector.size() ; i++){
        vertex_label_mapping[i] = v1[i];
    }
    vector<int> v2;
    copy(edge_vector.begin(), edge_vector.end(), back_inserter(v2));
    for(int i = 0 ; i < edge_vector.size() ; i++){
        edge_label_mapping[i] = v2[i];
    }

    vertex_bridge = new int *[v_size];
    for(int i = 0 ; i < v_size ; i++){
        vertex_bridge[i] = new int[2]; // 2 is number of possible edge label
        memset(vertex_bridge[i], 0, sizeof(int) * 2);
    }

    readFile.open(this->fileName);
    if(readFile.is_open()){
        char arr[256];
        readFile.getline(arr, 256); // skip graph num
        for(int i = 0 ; i < v_size ; i++){
            readFile.getline(arr, 256);
            split_vector = split(arr, ' ', "data");
            for(int i = 0 ; i < vertex_vector.size() ; i++){
                if(vertex_label_mapping[i] == split_vector[1]){
                    vertex_label_num[i]++;
                }
            }
        }
        for(int i = 0 ; i < e_size ; i++){
            readFile.getline(arr, 256);
            split_vector = split(arr, ' ', "data");
            vertex_bridge[split_vector[0]][split_vector[2]-1]++;
            vertex_bridge[split_vector[1]][split_vector[2]-1]++;
            g.set_edge(split_vector[0], split_vector[1], split_vector[2]);
            for(int i = 0 ; i < edge_vector.size() ; i++){
                if(edge_label_mapping[i] == split_vector[2]){
                    edge_label_num[i]++;
                }
            }
        }
    }
    readFile.close();
    g.set_graph_set(vertex_label_num, edge_label_num, vertex_set_size, edge_set_size, vertex_label_mapping, edge_label_mapping, vertex_bridge);
}

graph dataSet :: get_graph(){
    return this->g;
}

vector<int> dataSet :: split(string input, char delimiter, string type) {
    vector<int> result;
    if(!type.compare("data")){
        input = input.substr(2);
    }else if(!type.compare("graph_num")){
        input = input.substr(4);
    }
    stringstream ss(input);
    string temp;
    while (getline(ss, temp, delimiter)) {
        result.push_back(stoi(temp));
    }
    return result;
}