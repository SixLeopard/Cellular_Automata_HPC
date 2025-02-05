/*
======================================================
||               Ceullular Automata
======================================================
|| Jamie Westerhout 47451933
|| COSC3500 Sem2 2023
======================================================
*/
#ifndef LIFE_ARRAY_H

#define LIFE_ARRAY_H

#include "life.hpp"
#include "lifecore.hpp"
#include <mpi.h>

/*
======================================================
|| void get_value()
======================================================
|| state -> vectore state of the world to find value in 
|| y -> y position to get value at
|| x -> x poisiton to ge tvalue at
|| size -> the size of the array
======================================================
|| gets te value in array at (x,y) in the world
======================================================
*/
int get_value(VectorState& state, unsigned y, unsigned x, unsigned size){
    return state[y][x];
}

/*
======================================================
|| void resetWindow()
======================================================
|| window -> window array to reset
|| state -> array state to get value from
|| posx -> x position to center window at
|| posy -> y position to centre window at
======================================================
|| oupdates the values in the window to be centered
|| at a diffrent position
======================================================
*/
template<typename T>
void resetWindow(T window, VectorState& state, int posx, int posy, unsigned size){
    int left;
    int right;
    int up;
    int down;
    if (posx == 1) {
        left = size - 1;
        right = posx + 1;
    }
    else if (posx == size - 1) {
        left = posx - 1;
        right = 1;
    }
    else {
        left = posx - 1;
        right = posx + 1;
    }

    if (posy == 1) {
        up = size - 1;
        down = posy + 1;
    }
    else if (posy == size - 1) {
        up = posy - 1;
        down = 1;
    }
    else {
        up = posy - 1;
        down = posy + 1;
    }
    window[0] = state[up][left];
    window[1] = state[up][posx];
    window[2] = state[up][right];
    window[3] = state[posy][left];
    window[4] = state[posy][posx];
    window[5] = state[posy][right];
    window[6] = state[down][left];
    window[7] = state[down][posx];
    window[8] = state[down][right];
}

/*
======================================================
|| void evolve()
======================================================
|| state1 -> state that holds the current world state
|| state2 -> state to use a temproy storgae for future state
|| size -> the size to use for hashing
|| ruleSet -> the rule set to use for generation
======================================================
|| returns an update state1 and empty state2
======================================================
*/
void evolve(VectorState& state1, VectorState& state2, unsigned size, RuleSet& ruleSet, bool final = false){
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size); //get world size

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank); //get current rank of this node

    MPI_Status status; //status for recive dosent really get used
    MPI_Request request = MPI_REQUEST_NULL;

    int N2 = size*size; //N squared (number of cells in world)

    int N_per = size/world_size; //split size across nodes
    int offset = 0; //first needs to 1 so theres an edge buffer
    if (rank == 0) {
        offset = 1;
    }

    #pragma omp parallel for collapse(2) //open mp multi threading
    for (int y = N_per*rank + offset; y < N_per*(rank+1); y++){
        for (int x = 1; x < size; x++){
            u_int8_t window[WINDOWSIZE];
            resetWindow(window, state1, x, y, size);
            const auto weight = calculateWeight(window);
            state2[y][x] = testWeight(weight, state1[y][x], ruleSet);
        }
    }

    //switch vector to C array for easier communication between nodes
    unsigned* arr_state = (unsigned*) malloc(sizeof(unsigned) * size * size);

    for (unsigned i = 0; i < size; i++) {
        for (unsigned j = 0; j < size; j++) {
            arr_state[i*size+j] = state2[i][j];
        }
    }

   if (rank == 0){ //main task (runs only once)
        float* other_result_array = (float*)malloc(size*size*sizeof(unsigned));
        for (int other_task = 1; other_task < world_size; other_task++){
            //recive state from other tasks
            MPI_Recv(other_result_array, size * size, MPI_UNSIGNED, other_task, 1000+other_task, MPI_COMM_WORLD, &status);
            //update values
            for (int j = N_per*other_task; j < N_per*(other_task+1); j++){
                for (int i = 0; i < size; i++){
                    arr_state[i+size*j] = other_result_array[i+size*j];
                }
            }
        }
        free(other_result_array);
    } else { //other tasks
        MPI_Send(arr_state, size * size, MPI_UNSIGNED, 0, 1000+rank, MPI_COMM_WORLD); //send values for stae
    }

    if (rank == 0){
        for (int other_task = 1; other_task < world_size; other_task++){ //send final value for sate to all tasks
            MPI_Send(arr_state, size * size, MPI_FLOAT, other_task, 1000+other_task, MPI_COMM_WORLD);
        }
    } else {
        MPI_Recv(arr_state, size * size, MPI_UNSIGNED, 0, 1000+rank, MPI_COMM_WORLD, &status); //reviec complete state
    }

    //communications over, return state beack to vector
    for (unsigned i = 0; i < size; i++) {
        for (unsigned j = 0; j < size; j++) {
            state2[i][j] = arr_state[i*size+j];
        }
    }
    //normal swap
    std::swap(state1, state2);

    free(arr_state); //free array state

    return;
}

#endif