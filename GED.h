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
    int * vertex_mapping;
    int * edge_mapping;
    int vertex_mapping_size;
    int edge_mapping_size;
    int * GED_mapping;
    int index_mapping(int index);
    int index_unmapping(int index);
    bool is_full_mapping();
    int get_edit_cost();
    void print();
    void calculate_GED();
    void set_graph_set();
    void set_vertex_decrease(int id1, int id2);
    void set_vertex_increase(int id1, int id2);
    void set_edge_decrease(int id1, int id2);
    void set_edge_increase(int id1, int id2);
    int get_vertex_unmapped_part_cost();
    int get_edge_unmapped_part_cost();
    mapping copy_mapping(int i, int cost);
    int * temp_g1_edge_set;
    int * temp_g2_edge_set;
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
        // for(int i = 0 ; i < vertex_mapping_size ; i++){
        //     cout << g1_vertex_set[i] << " ";
        // }
        // cout << endl;
        // for(int i = 0 ; i < vertex_mapping_size ; i++){
        //     cout << g2_vertex_set[i] << " ";
        // }
        // cout << endl;
        // for(int i = 0 ; i < edge_mapping_size ; i++){
        //     cout << g1_edge_set[i] << " ";
        // }
        // cout << endl;
        // for(int i = 0 ; i < edge_mapping_size ; i++){
        //     cout << g2_edge_set[i] << " ";
        // }
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

int GED :: index_mapping(int index){
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

int GED :: index_unmapping(int index){
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

int GED :: get_edit_cost(){
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

void GED :: set_vertex_increase(int id1, int id2){
    for(int i = 0 ; i < vertex_mapping_size ; i++){
        if(vertex_mapping[i] == g1.get_vertex_label(id1)){
            g1_vertex_set[i]++;
        }
        if(vertex_mapping[i] == g2.get_vertex_label(id2)){
            g2_vertex_set[i]++;
        }
    }
}

void GED :: set_vertex_decrease(int id1, int id2){
    for(int i = 0 ; i < vertex_mapping_size ; i++){
        if(vertex_mapping[i] == g1.get_vertex_label(id1) && g1_vertex_set[i] != 0){
            g1_vertex_set[i]--;
        }
        if(vertex_mapping[i] == g2.get_vertex_label(id2) && g2_vertex_set[i] != 0){
            g2_vertex_set[i]--;
        }
    }
}

void GED :: set_edge_increase(int id1, int id2){
    for(int i = 0 ; i < edge_mapping_size ; i++){
        for(int j = 0 ; j < id1 ; j++){
            if(index_array[id1] == -1 || index_array[j] == -1){
                continue;
            }
            if(edge_mapping[i] == g1.get_edge_label(id1, j)){
                g1_edge_set[i]++;
            }
            if(edge_mapping[i] == g2.get_edge_label(index_array[id1], index_array[j])){
                g2_edge_set[i]++;
            }
        }
    }
}

void GED :: set_edge_decrease(int id1, int id2){
    for(int i = 0 ; i < edge_mapping_size ; i++){
        for(int j = 0 ; j < id1 ; j++){
            if(edge_mapping[i] == g1.get_edge_label(id1, j) && g1_edge_set[i] != 0){
                g1_edge_set[i]--;
            }
            if(edge_mapping[i] == g2.get_edge_label(index_array[id1], index_array[j]) && g2_edge_set[i] != 0){
                g2_edge_set[i]--;
            }
        }
    }
}

int GED :: get_vertex_unmapped_part_cost(){
    int cost = 0;
    for(int i = 0 ; i < vertex_mapping_size ; i++){
        int temp = g1_vertex_set[i] > g2_vertex_set[i] ? g1_vertex_set[i] - g2_vertex_set[i] : g2_vertex_set[i] - g1_vertex_set[i];
        cost += temp;
    }
    return cost;
}

int GED :: get_edge_unmapped_part_cost(){
    int cost = 0;
    for(int i = 0 ; i < edge_mapping_size ; i++){
        int temp = g1_edge_set[i] > g2_edge_set[i] ? g1_edge_set[i] - g2_edge_set[i] : g2_edge_set[i] - g1_edge_set[i];
        cost += temp;
    }
    return cost;
}

void GED :: calculate_GED(){
    priority_queue<mapping> q;
    for(int i = 0 ; i < max_size ; i++){
        if(search_array[i]){
            int j = index_mapping(i);
            set_vertex_decrease(j, i);
            set_edge_decrease(j, i);

            int cost2 = get_vertex_unmapped_part_cost() + get_edge_unmapped_part_cost();
            int edit_cost = get_edit_cost();

            set_vertex_increase(j, i);
            set_edge_increase(j, i);
            index_unmapping(i);
            if(cost2 + edit_cost > 5){
                continue;
            }
            Index index = Index(i, edit_cost + cost2);
            mapping m = copy_mapping(i, edit_cost + cost2);
            q.push(m);
        }
    }
    while(!q.empty()){
        mapping index = q.top();
        q.pop();
        int id = index_mapping(index.index_id);
        set_vertex_decrease(id, index.index_id);
        set_edge_decrease(id, index.index_id);

        if(min_cost != -1 && min_cost <= index.cost){
            set_vertex_increase(id, index.index_id);
            set_edge_increase(id, index.index_id);
            index_unmapping(index.index_id);
            return;
        }

        if(index_array[max_size-1] != -1){
            if(min_cost == -1){
                min_cost = index.cost;
                copy(&index_array[0], &index_array[max_size], &GED_mapping[0]);
            }else{
                if(min_cost >= index.cost){
                    min_cost = index.cost;
                    copy(&index_array[0], &index_array[max_size], &GED_mapping[0]);
                }
            }
        }

        calculate_GED();
        index_unmapping(index.index_id);
        set_vertex_increase(id, index.index_id);
        set_edge_increase(id, index.index_id);
    }
}

int GED :: get_GED(){
    start = clock();
    calculate_GED();
    end = clock();
    if(min_cost == -1){
        cout << "GED over 5" << endl;
        cout << min_cost << endl;
    }else{
        cout << "mapping" << endl;
        for(int i = 0 ; i < max_size ; i++){
            cout << i << " - " << GED_mapping[i] << endl;
        }
        cout << "time : " << (double)(end - start) / CLOCKS_PER_SEC << "s" << endl;
        cout << "GED : " << min_cost << endl;
    }
    return min_cost;
}

mapping GED :: copy_mapping(int i, int cost){
    mapping m = mapping(i, cost, max_size, vertex_mapping_size, edge_mapping_size);
    m.set_search_array(search_array);
    m.set_index_array(index_array);
    m.set_max_size(max_size);
    m.set_min_cost(min_cost);
    m.set_g1_vertex_set(g1_vertex_set);
    m.set_g1_edge_set(g1_edge_set);
    m.set_g2_vertex_set(g2_vertex_set);
    m.set_g2_edge_set(g2_edge_set);
    m.set_vertex_mapping(vertex_mapping);
    m.set_edge_mapping(edge_mapping);
    m.set_vertex_mapping_size(vertex_mapping_size);
    m.set_edge_mapping_size(edge_mapping_size);
    m.set_GED_mapping(GED_mapping);
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