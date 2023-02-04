#include<iostream>
#include<queue>

class mapping{
private : 
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
public : 
    int cost;
    int index_id;
    bool *search_array;
    int *index_array;
    int max_size;
    mapping(int i, int cost, int max_size, int vertex_mapping_size, int edge_mapping_size){
        this->index_id = i;
        this->cost = cost;
        search_array = new bool[max_size];
        memset(search_array, true, sizeof(bool) * max_size);
        index_array = new int[max_size];
        memset(index_array, -1, sizeof(int) * max_size);
        GED_mapping = new int[max_size];
        g1_vertex_set = new int[vertex_mapping_size];
        g1_edge_set = new int[edge_mapping_size];
        g2_vertex_set = new int[vertex_mapping_size];
        g2_edge_set = new int[edge_mapping_size];
        vertex_mapping = new int[vertex_mapping_size];
        edge_mapping = new int[edge_mapping_size];
    }
    mapping(int i, int cost, int max_size){
        this->index_id = i;
        this->cost = cost;
        this->max_size = max_size;
        search_array = new bool[max_size];
        memset(search_array, true, sizeof(bool) * max_size);
        index_array = new int[max_size];
        memset(index_array, -1, sizeof(int) * max_size);
    }
    void set_search_array(bool *search_array);
    void set_index_array(int *index_array);
    void set_max_size(int max_size);
    void set_min_cost(int min_cost);
    void set_g1_vertex_set(int * g1_vertex_set);
    void set_g1_edge_set(int * g1_edge_set);
    void set_g2_vertex_set(int * g2_vertex_set);
    void set_g2_edge_set(int * g2_edge_set);
    void set_vertex_mapping(int * vertex_mapping);
    void set_edge_mapping(int * edge_mapping);
    void set_vertex_mapping_size(int vertex_mapping_size);
    void set_edge_mapping_size(int edge_mapping_size);
    void set_GED_mapping(int * GED_mapping);

    bool operator<(const mapping mapping) const {
        return this->cost > mapping.cost;
    }
};

void mapping :: set_search_array(bool *search_array){
    memcpy(this->search_array, search_array, sizeof(bool) * max_size);
}

void mapping :: set_index_array(int *index_array){
    memcpy(this->index_array, index_array, sizeof(int) * max_size);
}

void mapping :: set_max_size(int max_size){

}

void mapping :: set_min_cost(int min_cost){

}

void mapping :: set_g1_vertex_set(int * g1_vertex_set){

}

void mapping :: set_g1_edge_set(int * g1_edge_set){

}

void mapping :: set_g2_vertex_set(int * g2_vertex_set){

}

void mapping :: set_g2_edge_set(int * g2_edge_set){

}

void mapping :: set_vertex_mapping(int * vertex_mapping){

}

void mapping :: set_edge_mapping(int * edge_mapping){

}

void mapping :: set_vertex_mapping_size(int vertex_mapping_size){

}

void mapping :: set_edge_mapping_size(int edge_mapping_size){

}

void mapping :: set_GED_mapping(int * GED_mapping){

}

