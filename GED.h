#include<iostream>
#include<ctime>
#include<chrono>
#include<algorithm>
#include"mapping.h"

using namespace std;
using namespace chrono;

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

    int ** g1_vertex_bridge;
    int ** g2_vertex_bridge;
    int ** temp_g1_vertex_bridge;
    int ** temp_g2_vertex_bridge;

    int * vertex_mapping;
    int * edge_mapping;
    int vertex_mapping_size;
    int edge_mapping_size;
    int * GED_mapping;

    int * ordering;

    int count = 0;

    int index_mapping(int index, int * index_array, bool * search_array);
    int index_unmapping(int index, int * index_array, bool * search_array);
    bool is_full_mapping(int * index_array);
    int get_edit_cost(int * index_array);
    void print();
    void calculate_GED();
    void set_graph_set();

    void set_vertex_decrease(int * index_array);
    void set_vertex_increase();
    void set_edge_decrease(int * index_array);
    void set_edge_increase();
    void set_bridge_decrease(int * index_array);
    void set_bridge_increase();

    void set_vertex_decrease_child(int g1_index, int g2_index);
    void set_vertex_increase_child(int g1_index, int g2_index);
    void set_edge_decrease_child(int g1_index, int * index_array);
    void set_edge_increase_child(int g1_index, int * index_array);
    void set_bridge_decrease_child(int g1_index, int * index_array);
    void set_bridge_increase_child(int g1_index, int * index_array);

    int get_vertex_unmapped_part_cost();
    int get_edge_unmapped_part_cost();
    int get_bridge_cost(int * index_array);

    double get_vertex_weight(int v);
    double get_edge_weight(int v1, int v2);
    void reorder();

    void swap(int n1, int n2);

    mapping copy_mapping(int i, int cost, int * index_array, bool * search_array, int vertex_hx, int edge_hx, int depth);
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

        g1_vertex_bridge = new int *[max_size];
        g2_vertex_bridge = new int *[max_size];
        temp_g1_vertex_bridge = new int *[max_size];
        temp_g2_vertex_bridge = new int *[max_size];
        for(int i = 0 ; i < max_size ; i++){
            g1_vertex_bridge[i] = new int[2];
            memset(g1_vertex_bridge[i], 0, sizeof(int) * 2);
            g2_vertex_bridge[i] = new int[2];
            memset(g2_vertex_bridge[i], 0, sizeof(int) * 2);
            temp_g1_vertex_bridge[i] = new int[2];
            temp_g2_vertex_bridge[i] = new int[2];
        }
        memcpy(g1_vertex_bridge, g1.vertex_bridge, sizeof(int) * g1.get_v_size() * 2);
        memcpy(g2_vertex_bridge, g2.vertex_bridge, sizeof(int) * g2.get_v_size() * 2);
        for(int i = 0 ; i < max_size ; i++){
            for(int j = 0 ; j < 2 ; j++){
                temp_g1_vertex_bridge[i][j] = g1_vertex_bridge[i][j];
                temp_g2_vertex_bridge[i][j] = g2_vertex_bridge[i][j];
            }
        }

        ordering = new int[max_size];
        for(int i = 0 ; i < max_size ; i++){
            ordering[i] = i;
        }
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

void GED :: swap(int n1, int n2){
    int temp = ordering[n1];
    ordering[n1] = ordering[n2];
    ordering[n2] = temp;
}

int GED :: index_mapping(int index, int * index_array, bool * search_array){
    int n = -1;
    if(index < max_size){
        search_array[index] = false;
        for(int i1 = 0 ; i1 < max_size ; i1++){
            int i = ordering[i1];
            if(index_array[i] == -1){
                index_array[i] = index;
                n = i1;
                break;
            }
        }
    }
    return n;
}

int GED :: index_unmapping(int index, int * index_array, bool * search_array){
    search_array[index] = true;
    for(int i1 = 0 ; i1 < max_size ; i1++){
        int i = ordering[i1];
        if(index_array[i] == index){
            index_array[i] = -1;
        }
    }
    return 0;
}

bool GED :: is_full_mapping(int * index_array){
    bool result = true;
    for(int i1 = 0 ; i1 < max_size ; i1++){
        int i = ordering[i1];
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
    for(int i1 = 0 ; i1 < max_size ; i1++){
        int i = ordering[i1];
        if(index_array[i] != -1){
            if(g1.get_vertex_label(i) != g2.get_vertex_label(index_array[i])){
                cost++;
            }
        }
    }
    for(int i1 = 1 ; i1 < max_size ; i1++){
        int i = ordering[i1];
        for(int j1 = i1-1 ; j1 >= 0 ; j1--){
            int j = ordering[j1];
            if(index_array[i] != -1 && index_array[j] != -1){
                if(g1.get_edge_label(i, j) != g2.get_edge_label(index_array[i], index_array[j])){
                    cost++;
                }
            }
        }
    }
    return cost;
}

void GED :: set_vertex_increase(){
    for(int i = 0 ; i < vertex_mapping_size ; i++){
        temp_g1_vertex_set[i] = g1_vertex_set[i];
        temp_g2_vertex_set[i] = g2_vertex_set[i];
    }
}

void GED :: set_vertex_decrease(int * index_array){
    for(int i1 = 0 ; i1 < max_size ; i1++){
        int i = ordering[i1];
        if(index_array[i] != -1){
            for(int j = 0 ; j < vertex_mapping_size ; j++){
                if(vertex_mapping[j] == g1.get_vertex_label(i) && temp_g1_vertex_set[j] != 0){
                    temp_g1_vertex_set[j]--;
                }
                if(vertex_mapping[j] == g2.get_vertex_label(index_array[i]) && temp_g2_vertex_set[j] != 0){
                    temp_g2_vertex_set[j]--;
                }
            }
        }
        
    }
}

void GED :: set_edge_increase(){
    for(int i = 0 ; i < edge_mapping_size ; i++){
        temp_g1_edge_set[i] = g1_edge_set[i];
        temp_g2_edge_set[i] = g2_edge_set[i];
    }
}

void GED :: set_edge_decrease(int * index_array){
    for(int i1 = 1 ; i1 < max_size ; i1++){
        int i = ordering[i1];
        for(int j1 = i1-1 ; j1 >= 0 ; j1--){
            int j = ordering[j1];
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

void GED :: set_bridge_increase(){
    for(int i1 = 0 ; i1 < max_size ; i1++){
        int i = ordering[i1];
        for(int j = 0 ; j < 2 ; j++){
            temp_g1_vertex_bridge[i][j] = g1_vertex_bridge[i][j];
            temp_g2_vertex_bridge[i][j] = g2_vertex_bridge[i][j];
        }
    }
}

void GED :: set_bridge_decrease(int * index_array){
    for(int i1 = 1 ; i1 < max_size ; i1++){
        int i = ordering[i1];
        for(int j1 = i1-1 ; j1 >= 0 ; j1--){
            int j = ordering[j1];
            if(index_array[i] != -1 && index_array[j] != -1){
                if(g1.get_edge_label(i, j) != 0){
                    temp_g1_vertex_bridge[i][g1.get_edge_label(i, j)-1]--;
                    temp_g1_vertex_bridge[j][g1.get_edge_label(i, j)-1]--;
                }
                if(g2.get_edge_label(index_array[i], index_array[j]) != 0){
                    temp_g2_vertex_bridge[index_array[i]][g2.get_edge_label(index_array[i], index_array[j])-1]--;
                    temp_g2_vertex_bridge[index_array[j]][g2.get_edge_label(index_array[i], index_array[j])-1]--;
                }
            }
        }
    }
}

void GED :: set_vertex_increase_child(int g1_index, int g2_index){
    g1_index = ordering[g1_index];
    for(int i = 0 ; i < vertex_mapping_size ; i++){
        if(vertex_mapping[i] == g1.get_vertex_label(g1_index)){
            temp_g1_vertex_set[i]++;
        }
        if(vertex_mapping[i] == g2.get_vertex_label(g2_index)){
            temp_g2_vertex_set[i]++;
        }
    }
}

void GED :: set_vertex_decrease_child(int g1_index, int g2_index){
    g1_index = ordering[g1_index];
    for(int i = 0 ; i < vertex_mapping_size ; i++){
        if(vertex_mapping[i] == g1.get_vertex_label(g1_index) && g1_vertex_set[i] != 0){
            temp_g1_vertex_set[i]--;
        }
        if(vertex_mapping[i] == g2.get_vertex_label(g2_index) && g2_vertex_set[i] != 0){
            temp_g2_vertex_set[i]--;
        }
    }
}

void GED :: set_edge_increase_child(int g1_index, int * index_array){
    for(int i = 0 ; i < edge_mapping_size ; i++){
        for(int j1 = 0 ; j1 < g1_index ; j1++){
            int j = ordering[j1];
            if(index_array[ordering[g1_index]] == -1 || index_array[j] == -1){
                continue;
            }
            if(edge_mapping[i] == g1.get_edge_label(ordering[g1_index], j)){
                temp_g1_edge_set[i]++;
            }
            if(edge_mapping[i] == g2.get_edge_label(index_array[ordering[g1_index]], index_array[j])){
                temp_g2_edge_set[i]++;
            }
        }
    }
}

void GED :: set_edge_decrease_child(int g1_index, int * index_array){
    for(int i = 0 ; i < edge_mapping_size ; i++){
        for(int j1 = 0 ; j1 < g1_index ; j1++){
            int j = ordering[j1];
            if(edge_mapping[i] == g1.get_edge_label(ordering[g1_index], j) && temp_g1_edge_set[i] != 0){
                temp_g1_edge_set[i]--;
            }
            if(edge_mapping[i] == g2.get_edge_label(index_array[ordering[g1_index]], index_array[j]) && temp_g2_edge_set[i] != 0){
                temp_g2_edge_set[i]--;
            }
        }
    }
}

void GED :: set_bridge_increase_child(int g1_index, int * index_array){
    int g1_index1 = ordering[g1_index];
    for(int j1 = g1_index-1 ; j1 >= 0 ; j1--){
        int j = ordering[j1];
        if(g1.get_edge_label(g1_index1, j) != 0){
            temp_g1_vertex_bridge[g1_index1][g1.get_edge_label(g1_index1, j)-1]++;
            temp_g1_vertex_bridge[j][g1.get_edge_label(g1_index1, j)-1]++;
        }
        if(g2.get_edge_label(index_array[g1_index1], index_array[j]) != 0){
            temp_g2_vertex_bridge[index_array[g1_index1]][g2.get_edge_label(index_array[g1_index1], index_array[j])-1]++;
            temp_g2_vertex_bridge[index_array[j]][g2.get_edge_label(index_array[g1_index1], index_array[j])-1]++;
        }
    }
}

void GED :: set_bridge_decrease_child(int g1_index, int * index_array){
    int g1_index1 = ordering[g1_index];
    for(int j1 = g1_index-1 ; j1 >= 0 ; j1--){
        int j = ordering[j1];
        if(g1.get_edge_label(g1_index1, j) != 0){
            temp_g1_vertex_bridge[g1_index1][g1.get_edge_label(g1_index1, j)-1]--;
            temp_g1_vertex_bridge[j][g1.get_edge_label(g1_index1, j)-1]--;
        }
        if(g2.get_edge_label(index_array[g1_index1], index_array[j]) != 0){
            temp_g2_vertex_bridge[index_array[g1_index1]][g2.get_edge_label(index_array[g1_index1], index_array[j])-1]--;
            temp_g2_vertex_bridge[index_array[j]][g2.get_edge_label(index_array[g1_index1], index_array[j])-1]--;
        }
    }    
}

int GED :: get_vertex_unmapped_part_cost(){
    int g1_sum = 0;
    int g2_sum = 0;
    for(int i = 0 ; i < vertex_mapping_size ; i++){
        g1_sum += temp_g1_vertex_set[i];
        g2_sum += temp_g2_vertex_set[i];
    }

    for(int i = 0 ; i < vertex_mapping_size ; i++){
        int temp = temp_g1_vertex_set[i] < temp_g2_vertex_set[i] ? temp_g1_vertex_set[i] : temp_g2_vertex_set[i];
        g1_sum -= temp;
        g2_sum -= temp;
    }

    int cost = g1_sum > g2_sum ? g1_sum : g2_sum;
    return cost;
}

int GED :: get_edge_unmapped_part_cost(){
    int g1_sum = 0;
    int g2_sum = 0;
    for(int i = 0 ; i < edge_mapping_size ; i++){
        g1_sum += temp_g1_edge_set[i];
        g2_sum += temp_g2_edge_set[i];
    }

    for(int i = 0 ; i < edge_mapping_size ; i++){
        int temp = temp_g1_edge_set[i] < temp_g2_edge_set[i] ? temp_g1_edge_set[i] : temp_g2_edge_set[i];
        g1_sum -= temp;
        g2_sum -= temp;
    }

    int cost = g1_sum > g2_sum ? g1_sum : g2_sum;
    return cost;
}

int GED :: get_bridge_cost(int * index_array){
    int cost = 0;
    for(int i1 = 0 ; i1 < max_size ; i1++){
        int i = ordering[i1];
        if(index_array[i] != -1){
            // int temp1 = temp_g1_vertex_bridge[i][0] > temp_g2_vertex_bridge[index_array[i]][0] ? 
            // temp_g1_vertex_bridge[i][0] - temp_g2_vertex_bridge[index_array[i]][0] : 
            // temp_g2_vertex_bridge[index_array[i]][0] - temp_g1_vertex_bridge[i][0];

            // int temp2 = temp_g1_vertex_bridge[i][1] > temp_g2_vertex_bridge[index_array[i]][1] ? 
            // temp_g1_vertex_bridge[i][1] - temp_g2_vertex_bridge[index_array[i]][1] : 
            // temp_g2_vertex_bridge[index_array[i]][1] - temp_g1_vertex_bridge[i][1];
            
            // cost = cost + temp1 + temp2;

            int g1_sum = temp_g1_vertex_bridge[i][0] + temp_g1_vertex_bridge[i][1];
            int g2_sum = temp_g2_vertex_bridge[index_array[i]][0] + temp_g2_vertex_bridge[index_array[i]][1];
            int temp1 = temp_g1_vertex_bridge[i][0] > temp_g2_vertex_bridge[index_array[i]][0] ? temp_g2_vertex_bridge[index_array[i]][0] : temp_g1_vertex_bridge[i][0];
            int temp2 = temp_g1_vertex_bridge[i][1] > temp_g2_vertex_bridge[index_array[i]][1] ? temp_g2_vertex_bridge[index_array[i]][1] : temp_g1_vertex_bridge[i][1];
            g1_sum = g1_sum - temp1 - temp2;
            g2_sum = g2_sum - temp1 - temp2;
            int temp_cost = g1_sum > g2_sum ? g1_sum : g2_sum;
            cost += temp_cost;

        }
    }
    // cout << cost << endl;
    return cost;
}

double GED :: get_vertex_weight(int v){
    int vfreq = 0;
    for(int i = 0 ; i < vertex_mapping_size ; i++){
        if(g1.get_vertex_label(ordering[v]) == vertex_mapping[i]){
            vfreq = g1_vertex_set[i];
        }
    }
    double weight = 1.0 - (double)vfreq / g1.get_v_size();
    return weight;
}

double GED :: get_edge_weight(int v1, int v2){
    int efreq = 0;
    for(int i = 0 ; i < edge_mapping_size ; i++){
        if(g1.get_edge_label(ordering[v1], ordering[v2]) == edge_mapping[i]){
            efreq = g1_edge_set[i];
        }
    }
    double weight = 1.0 - (double)efreq / g1.get_e_size();
    weight = (weight == 1) ? 0 : weight;
    return weight;
}

void GED :: reorder(){
    int v = 0;

    int * temp_vertex_set = new int[g1.vertex_set_size];
    for(int i = 0 ; i < g1.vertex_set_size ; i++){
        temp_vertex_set[i] = g1.vertex_set[i];
    }
    sort(temp_vertex_set, temp_vertex_set + g1.vertex_set_size);
    cout << endl;
    int min = 0;
    for(int i = 0 ; i < g1.vertex_set_size ; i++){
        if(min + temp_vertex_set[i] >= temp_vertex_set[g1.vertex_set_size-1]) break;
        min += temp_vertex_set[i];
    }
    // cout << min << endl;
    // double vw = (double)g1.get_v_size() / ((double)g1_vertex_set[1] + (double)g1_vertex_set[2]);
    int edge_set_min = g1_edge_set[0] > g1_edge_set[1] ? g1_edge_set[1] : g1_edge_set[0];\

    // min = 1;
    // edge_set_min = 1;

    double vw = (double)g1.get_v_size() / (double)min;
    double ew = (double)g1.get_e_size() / (double)edge_set_min;
    cout << vw << endl;
    cout << ew << endl;
    double max = 0.0;
    for(int i = 0 ; i < g1.get_v_size() ; i++){
        double weight = get_vertex_weight(i) * vw;
        // cout << i << " " << weight << endl;
        for(int j = 0 ; j < g1.get_v_size() ; j++){
            weight += get_edge_weight(i, j) * ew;
        }
        // cout << i << " " << weight << endl;
        if(weight > max){
            max = weight;
            v = i;
        }
    }
    swap(0, v);
    // cout << "----------------" << endl;
    for(int next = 1 ; next < g1.get_v_size() ; next++){
        max = 0.0;
        for(int i = next ; i < g1.get_v_size() ; i++){
            double weight = 0.0;
            for(int j = 0 ; j < g1.get_v_size() ; j++){
                weight += get_edge_weight(i, j) * ew;
            }
            if(weight != 0.0){
                weight += get_vertex_weight(i) * vw;
            }
            if(weight > max){
                max = weight;
                v = i;
            }
            // cout << i << " " << weight << endl;
        }
        // cout <<  "----------------" << endl;
        if(max == 0.0){
            for(int i = next ; i < g1.get_v_size() ; i++){
                double weight = get_vertex_weight(i)*vw;
                for(int j = next ; j < g1.get_v_size() ; j++){
                    weight += get_edge_weight(i, j) * ew;
                }
                if(weight > max){
                    max = weight;
                    v = i;
                }
            }
        }
        swap(next, v);
    }
}

void GED :: calculate_GED(){
    priority_queue<mapping> q;
    for(int i = 0 ; i < max_size ; i++){
        int j = index_mapping(i, index_array, search_array);
        int depth = 1;
        set_vertex_decrease(index_array);
        set_edge_decrease(index_array);

        int edit_cost = get_edit_cost(index_array);
        int vertex_hx_cost = get_vertex_unmapped_part_cost();
        int edge_hx_cost = get_edge_unmapped_part_cost();
        int hx_cost = vertex_hx_cost + edge_hx_cost;

        set_vertex_increase();
        set_edge_increase();

        mapping m = copy_mapping(i, edit_cost + hx_cost , index_array, search_array, vertex_hx_cost, edge_hx_cost, depth);
        index_unmapping(i, index_array, search_array);

        if(edit_cost + hx_cost > 5){
            continue;
        }
        
        q.push(m);
    }

    while(!q.empty()){
        mapping index = q.top();
        if(is_full_mapping(index.index_array)){
            min_cost = index.cost;
            copy(&index.index_array[0], &index.index_array[max_size], &GED_mapping[0]);
            break;
        }
        set_vertex_decrease(index.index_array);
        set_edge_decrease(index.index_array);
        set_bridge_decrease(index.index_array);
        q.pop();

        count++;
        for(int i = 0 ; i < max_size ; i++){
            if(index.search_array[i]){
                int depth = index.depth++;
                int g1_index = index_mapping(i, index.index_array, index.search_array);
                set_vertex_decrease_child(g1_index, i);
                set_edge_decrease_child(g1_index, index.index_array);
                set_bridge_decrease_child(g1_index, index.index_array);

                int edit_cost = get_edit_cost(index.index_array);
                int vertex_hx_cost = get_vertex_unmapped_part_cost();
                int edge_hx_cost = get_edge_unmapped_part_cost();
                int bridge_cost = get_bridge_cost(index.index_array);
                int hx_cost = vertex_hx_cost + edge_hx_cost + 0;

                set_bridge_increase_child(g1_index, index.index_array);
                set_edge_increase_child(g1_index, index.index_array);
                set_vertex_increase_child(g1_index, i);

                mapping m = copy_mapping(i, edit_cost + hx_cost, index.index_array, index.search_array, vertex_hx_cost, edge_hx_cost, depth);
                index_unmapping(i, index.index_array, index.search_array);
                if(edit_cost + hx_cost > 5){
                    continue;
                }
                
                q.push(m);
            }
        }

        set_bridge_increase();
        set_edge_increase();
        set_vertex_increase();

    }
}

int GED :: get_GED(){
    reorder();
    
    for(int i = 0 ; i < max_size ; i++){
        cout << ordering[i] << " ";
    }
    cout << endl;

    start = clock();
    system_clock::time_point start_time = system_clock::now();
    calculate_GED();
    system_clock::time_point end_time = system_clock::now();
    end = clock();
    microseconds micro = duration_cast<microseconds>(end_time - start_time);
    if(min_cost == -1){
        cout << "GED over 5" << endl;
    }else{
        cout << "mapping" << endl;
        for(int i = 0 ; i < max_size ; i++){
            cout << i << " - " << GED_mapping[i] << endl;
        }
        cout << "time : " << (double)(end - start) / CLOCKS_PER_SEC << "s" << endl;
        cout << "걸린 시간 : " << micro.count() << " microseconds" << endl;
        cout << "pop한 횟수 : " << count << " 번" << endl;
        cout << "GED : " << min_cost << endl;
    }

    return min_cost;
}

mapping GED :: copy_mapping(int i, int cost, int * index_array, bool * search_array, int vertex_hx, int edge_hx, int depth){
    mapping m = mapping(i, cost, max_size, vertex_hx, edge_hx, depth);
    m.set_search_array(search_array);
    m.set_index_array(index_array);
    return m;
}

void GED :: test(){
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
    // cout << endl ;
    // for(int i = 0 ; i < max_size ; i++){
    //     cout << index_array[i] << " " << endl;
    // }
    // cout << endl<< endl;
    // for(int i = 0 ; i < vertex_mapping_size ; i++){
    //     cout << vertex_mapping[i] << endl;
    //     cout << g1_vertex_set[i] << endl;
    // }
    for(int i = 0 ; i < max_size ; i++){
        cout << temp_g2_vertex_bridge[i][0] << " " << temp_g2_vertex_bridge[i][1] << endl;
    }
}