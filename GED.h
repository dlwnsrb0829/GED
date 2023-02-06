#include<iostream>
#include<ctime>
#include"mapping.h"

using namespace std;

struct Index
{
    int index_id;
    int cost;
    Index(int index_id, int cost){
        this->index_id = index_id;
        this->cost = cost;
    }
    bool operator<(const Index index) const {
        return this->cost > index.cost;
    }
};

class GED{
private : 
    graph g1;
    graph g2;
    bool *search_array;
    int *index_array;
    int max_size;
    int min_cost;
    int start, end;
    int * g1_vertex_set;
    int * g1_edge_set;
    int * g2_vertex_set;
    int * g2_edge_set;

    int * temp_g1_vertex_set;
    int * temp_g1_edge_set;
    int * temp_g2_vertex_set;
    int * temp_g2_edge_set;

    int * vertex_mapping;
    int * edge_mapping;
    int vertex_mapping_size;
    int edge_mapping_size;
    int * GED_mapping;
    int index_mapping(int index, int * index_array, bool * search_array);
    int index_unmapping(int index, int * index_array, bool * search_array);
    bool is_full_mapping();
    int get_edit_cost(int * index_array);
    void print();
    void calculate_GED();
    void set_graph_set();
    void set_vertex_decrease(int id1, int id2);
    void set_vertex_increase(int id1, int id2);
    void set_edge_decrease(int id1, int id2);
    void set_edge_increase(int id1, int id2);

    void set_vertex_decrease(int * index_array, bool * search_array);
    void set_vertex_increase(int * index_array);
    void set_edge_decrease(int * index_array);
    void set_edge_increase(int * index_array);

    int get_vertex_unmapped_part_cost();
    int get_edge_unmapped_part_cost();
    mapping copy_mapping(int i, int cost, int * index_array, bool * search_array, int vertexhx, int edgehx);
    void test();

public :
    GED(graph g1, graph g2){
        this->g1 = g1;
        this->g2 = g2;
        this->max_size = g1.get_v_size() > g2.get_v_size() ? g1.get_v_size() : g2.get_v_size();
        search_array = new bool[max_size];
        memset(search_array, true, sizeof(bool) * max_size);
        index_array = new int[max_size];
        memset(index_array, -1, sizeof(int) * max_size);
        GED_mapping = new int[max_size];
        min_cost = -1;
        set_graph_set();
        temp_g1_edge_set = new int[edge_mapping_size];
        temp_g2_edge_set = new int[edge_mapping_size];
        temp_g1_vertex_set = new int[vertex_mapping_size];
        temp_g2_vertex_set = new int[vertex_mapping_size];
        memcpy(temp_g1_vertex_set, g1_vertex_set, sizeof(int) * vertex_mapping_size);
        memcpy(temp_g2_vertex_set, g2_vertex_set, sizeof(int) * vertex_mapping_size);
        memcpy(temp_g1_edge_set, g1_edge_set, sizeof(int) * edge_mapping_size);
        memcpy(temp_g2_edge_set, g2_edge_set, sizeof(int) * edge_mapping_size);
    }
    int get_GED();
};

void GED :: set_graph_set(){
    set<int> vertex_set_temp;
    set<int> edge_set_temp;
    set<int> vertex_mapping_set_temp;
    set<int> edge_mapping_set_temp;
    for(int i = 0 ; i < g1.vertex_set_size ; i++){
        vertex_mapping_set_temp.insert(g1.vertex_set_mapping[i]);
    }
    for(int i = 0 ; i < g2.vertex_set_size ; i++){
        vertex_mapping_set_temp.insert(g2.vertex_set_mapping[i]);
    }
    for(int i = 0 ; i < g1.edge_set_size ; i++){
        edge_mapping_set_temp.insert(g1.edge_set_mapping[i]);
    }
    for(int i = 0 ; i < g2.edge_set_size ; i++){
        edge_mapping_set_temp.insert(g2.edge_set_mapping[i]);
    }
    vector<int> vertex_mapping_vector_temp;
    vector<int> edge_mapping_vector_temp;
    copy(vertex_mapping_set_temp.begin(), vertex_mapping_set_temp.end(), back_inserter(vertex_mapping_vector_temp));
    copy(edge_mapping_set_temp.begin(), edge_mapping_set_temp.end(), back_inserter(edge_mapping_vector_temp));
    vertex_mapping_size = vertex_mapping_set_temp.size();
    edge_mapping_size = edge_mapping_set_temp.size();
    g1_vertex_set = new int[vertex_mapping_size];
    g1_edge_set = new int[edge_mapping_size];
    g2_vertex_set = new int[vertex_mapping_size];
    g2_edge_set = new int[edge_mapping_size];
    vertex_mapping = new int[vertex_mapping_size];
    edge_mapping = new int[edge_mapping_size];

    memset(g1_vertex_set, 0, sizeof(int) * vertex_mapping_size);
    memset(g2_vertex_set, 0, sizeof(int) * vertex_mapping_size);
    memset(g1_edge_set, 0, sizeof(int) * edge_mapping_size);
    memset(g2_edge_set, 0, sizeof(int) * edge_mapping_size);

    for(int i = 0 ; i < vertex_mapping_size ; i++){
        vertex_mapping[i] = vertex_mapping_vector_temp[i];
    }
    for(int i = 0 ; i < edge_mapping_size ; i++){
        edge_mapping[i] = edge_mapping_vector_temp[i];
    }

    for(int i = 0 ; i < vertex_mapping_size ; i++){
        for(int j = 0 ; j < g1.vertex_set_size ; j++){
            if(g1.vertex_set_mapping[j] == vertex_mapping_vector_temp[i]){
                g1_vertex_set[i] = g1.vertex_set[j];
            }
        }
        for(int j = 0 ; j < g2.vertex_set_size ; j++){
            if(g2.vertex_set_mapping[j] == vertex_mapping_vector_temp[i]){
                g2_vertex_set[i] = g2.vertex_set[j];
            }
        }
    }
    for(int i = 0 ; i < edge_mapping_size ; i++){
        for(int j = 0 ; j < g1.edge_set_size ; j++){
            if(g1.edge_set_mapping[j] == edge_mapping_vector_temp[i]){
                g1_edge_set[i] = g1.edge_set[j];
            }
        }
        for(int j = 0 ; j < g2.edge_set_size ; j++){
            if(g2.edge_set_mapping[j] == edge_mapping_vector_temp[i]){
                g2_edge_set[i] = g2.edge_set[j];
            }
        }
    }
}

int GED :: index_mapping(int index, int * index_array, bool * search_array){
    int n = -1;
    if(index < max_size){
        search_array[index] = false;
        for(int i = 0 ; i < max_size ; i++){
            if(index_array[i] == -1){
                index_array[i] = index;
                n = i;
                break;
            }
        }
    }
    return n;
}

int GED :: index_unmapping(int index, int * index_array, bool * search_array){
    search_array[index] = true;
    for(int i = 0 ; i < max_size ; i++){
        if(index_array[i] == index){
            index_array[i] = -1;
        }
    }
    return 0;
}

bool GED :: is_full_mapping(){
    bool result = true;
    for(int i = 0 ; i < max_size ; i++){
        if(index_array[i] == -1){
            result = false;
        }
    }
    return result;
}

void GED :: print(){
    for(int i = 0 ; i < max_size ; i++){
        if(index_array[i] != -1){
            cout << "g1 : " << i << ", g2 = " << index_array[i] << endl;
        }
    }
}

int GED :: get_edit_cost(int * index_array){
    int cost = 0;
    for(int i = 0 ; i < max_size ; i++){
        if(index_array[i] != -1){
            if(g1.get_vertex_label(i) != g2.get_vertex_label(index_array[i])){
                cost++;
            }
        }
    }
    for(int i = 1 ; i < max_size ; i++){
        for(int j = i-1 ; j >= 0 ; j--){
            if(index_array[i] != -1 && index_array[j] != -1){
                if(g1.get_edge_label(i, j) != g2.get_edge_label(index_array[i], index_array[j])){
                    cost++;
                }
            }
        }
    }
    return cost;
}

void GED :: set_vertex_increase(int * index_array){
    for(int i = 0 ; i < vertex_mapping_size ; i++){
        temp_g1_vertex_set[i] = g1_vertex_set[i];
        temp_g2_vertex_set[i] = g2_vertex_set[i];
    }
}

void GED :: set_vertex_decrease(int * index_array, bool * search_array){
    for(int i = 0 ; i < max_size ; i++){
        if(!search_array[i]){
            for(int j = 0 ; j < vertex_mapping_size ; j++){
                if(vertex_mapping[j] == g1.get_vertex_label(index_array[i]) && temp_g1_vertex_set[j] != 0){
                    temp_g1_vertex_set[j]--;
                }
                if(vertex_mapping[j] == g2.get_vertex_label(i) && temp_g2_vertex_set[j] != 0){
                    temp_g2_vertex_set[j]--;
                }
            }
        }
        
    }
}

void GED :: set_edge_increase(int id1, int id2){
    for(int i = 0 ; i < edge_mapping_size ; i++){
        temp_g1_edge_set[i] = g1_edge_set[i];
        temp_g2_edge_set[i] = g2_edge_set[i];
    }
}

void GED :: set_edge_decrease(int * index_array){
    for(int i = 1 ; i < max_size ; i++){
        for(int j = i-1 ; j >= 0 ; j--){
            if(index_array[i] != -1 && index_array[j] != -1){
                for(int k = 0 ; k < edge_mapping_size ; k++){
                    if(edge_mapping[k] == g1.get_edge_label(i, j) && temp_g1_edge_set[k] != 0){
                        temp_g1_edge_set[k]--;
                    }
                    if(edge_mapping[k] == g2.get_edge_label(index_array[i], index_array[j]) && temp_g2_edge_set[k] != 0){
                        temp_g2_edge_set[k]--;
                    }
                }
            }
        }
    }
}

int GED :: get_vertex_unmapped_part_cost(){
    int cost = 0;
    for(int i = 0 ; i < vertex_mapping_size ; i++){
        int temp = temp_g1_vertex_set[i] > temp_g2_vertex_set[i] ? temp_g1_vertex_set[i] - temp_g2_vertex_set[i] : temp_g2_vertex_set[i] - temp_g1_vertex_set[i];
        cost += temp;
    }
    return cost;
}

int GED :: get_edge_unmapped_part_cost(){
    int cost = 0;
    for(int i = 0 ; i < edge_mapping_size ; i++){
        int temp = temp_g1_edge_set[i] > temp_g2_edge_set[i] ? temp_g1_edge_set[i] - temp_g2_edge_set[i] : temp_g2_edge_set[i] - temp_g1_edge_set[i];
        cost += temp;
    }
    return cost;
}

void GED :: calculate_GED(){
    priority_queue<mapping> q;
    for(int i = 0 ; i < max_size ; i++){
        int j = index_mapping(i, index_array, search_array);
        set_vertex_decrease(index_array, search_array);
        set_edge_decrease(index_array);

        int edit_cost = get_edit_cost(index_array);
        int vertex_hx_cost = get_vertex_unmapped_part_cost();
        int edge_hx_cost = get_edge_unmapped_part_cost();
        int hx_cost = vertex_hx_cost + edge_hx_cost;

        set_vertex_increase(index_array);
        set_edge_increase(j, i);

        mapping m = copy_mapping(i, edit_cost + hx_cost , index_array, search_array, vertex_hx_cost, edge_hx_cost);
        index_unmapping(i, index_array, search_array);

        if(edit_cost + hx_cost > 10){
            continue;
        }
        
        q.push(m);
    }

    while(!q.empty()){
        mapping index = q.top();

        if(index.index_array[max_size-1] != -1){
            min_cost = index.cost;
            copy(&index.index_array[0], &index.index_array[max_size], &GED_mapping[0]);
            break;
        }

        q.pop();
        for(int i = 0 ; i < max_size ; i++){
            if(index.search_array[i]){
                int j = index_mapping(i, index.index_array, index.search_array);
                set_vertex_decrease(index.index_array, index.search_array);
                set_edge_decrease(index.index_array);

                int edit_cost = get_edit_cost(index.index_array);
                int vertex_hx_cost = get_vertex_unmapped_part_cost();
                int edge_hx_cost = get_edge_unmapped_part_cost();
                int hx_cost = vertex_hx_cost + edge_hx_cost;

                set_edge_increase(j, i);
                set_vertex_increase(index.index_array);

                mapping m = copy_mapping(i, edit_cost + hx_cost, index.index_array, index.search_array, vertex_hx_cost, edge_hx_cost);
                index_unmapping(i, index.index_array, index.search_array);

                if(edit_cost + hx_cost > 10){
                    continue;
                }

                q.push(m);
            }
        }

    }
}

int GED :: get_GED(){
    start = clock();
    calculate_GED();
    end = clock();
    if(min_cost == -1){
        cout << "GED over 5" << endl;
    }else{
        // cout << "mapping" << endl;
        // for(int i = 0 ; i < max_size ; i++){
        //     cout << i << " - " << GED_mapping[i] << endl;
        // }
        cout << "time : " << (double)(end - start) / CLOCKS_PER_SEC << "s" << endl;
        cout << "GED : " << min_cost << endl;
    }
    return min_cost;
}

mapping GED :: copy_mapping(int i, int cost, int * index_array, bool * search_array, int vertex_hx, int edge_hx){
    mapping m = mapping(i, cost, max_size, vertex_hx, edge_hx);
    m.set_search_array(search_array);
    m.set_index_array(index_array);
    return m;
}

void GED :: test(){
    for(int i = 0 ; i < vertex_mapping_size ; i++){
        cout << g1_vertex_set[i] << " ";
    }
    cout << endl;
    for(int i = 0 ; i < vertex_mapping_size ; i++){
        cout << g2_vertex_set[i] << " ";
    }
    cout << endl;
    for(int i = 0 ; i < edge_mapping_size ; i++){
        cout << g1_edge_set[i] << " ";
    }
    cout << endl;
    for(int i = 0 ; i < edge_mapping_size ; i++){
        cout << g2_edge_set[i] << " ";
    }
    cout << endl ;
    for(int i = 0 ; i < max_size ; i++){
        cout << index_array[i] << " " << endl;
    }
    cout << endl<< endl;
}