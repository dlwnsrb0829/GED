#include<iostream>
#include<queue>

class mapping{
private : 
    int min_cost;
public : 
    int cost;
    int vertex_hx;
    int edge_hx;
    int index_id;
    bool *search_array;
    int *index_array;
    int max_size;
    mapping(int i, int cost, int max_size, int vertex_hx, int edge_hx){
        this->index_id = i;
        this->cost = cost;
        this->max_size = max_size;

        this->vertex_hx = vertex_hx;
        this->edge_hx = edge_hx;

        search_array = new bool[max_size];
        memset(search_array, true, sizeof(bool) * max_size);
        index_array = new int[max_size];
        memset(index_array, -1, sizeof(int) * max_size);
    }
    void set_search_array(bool *search_array);
    void set_index_array(int *index_array);
    void set_max_size(int max_size);
    void set_min_cost(int min_cost);

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