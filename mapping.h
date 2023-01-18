#include<iostream>
#include<queue>

class mapping{
private : 
    bool *search_array;
    int *index_array;
    int size;
public : 
    mapping(int size){
        search_array = new bool[size];
        memset(search_array, false, sizeof(bool) * size);
        index_array = new int[size];
        memset(index_array, -1, sizeof(int) * size);
        this->size = size;
    }
    int* index_mapping(int index);

};

int* mapping :: index_mapping(int index){
    if(index < size){
        // search_array[index] = true;
        for(int i = 0 ; i < size ; i++){
            if(index_array[i] == -1){
                index_array[i] = index;
                break;
            }
        }
    }
    return index_array;
}