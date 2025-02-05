/*
======================================================
||               Ceullular Automata
======================================================
|| Jamie Westerhout 47451933
|| COSC3500 Sem2 2023
======================================================
*/
#ifndef LIFE_HASH_H

#define LIFE_HASH_H

#define HASH_SCALER 4

#include "life.hpp"
#include "lifecore.hpp"

/*
======================================================
|| void posHash()
======================================================
|| y -> y position to hash
|| x -> x position to hash
|| size -> size to use when hashing
======================================================
|| hash the input position into an int
======================================================
*/
long posHash(int y, int x, long size){
    long scale = size*HASH_SCALER;
    return y*(scale)+x;
}

/*
======================================================
|| void posHash()
======================================================
|| hashed -> hashed position to unhash
|| size -> size used when hashing
======================================================
|| takes a hashed position and returns the  
|| x and y position
======================================================
*/
std::pair<int, int> posUnHash(long hashed, long size){
    int y = floor(hashed/(size*HASH_SCALER));
    int x = hashed - (y*size*HASH_SCALER);
    return std::make_pair(y,x);
}

/*
======================================================
|| void get_value()
======================================================
|| state ->the current world state
|| y -> y position to find value at
|| x -> x position to find value at
|| size -> size used when hashing
======================================================
|| returns the value at (x,y) in the world
======================================================
*/
int get_value(HashState& state, int y, int x, long size){
    return state[posHash(y,x,size)];
}

/*
======================================================
|| void get_value()
======================================================
|| state ->the current world state
|| hash -> hashvalue to find value at
|| size -> size used when hashing
======================================================
|| returns the value at hash in the world
======================================================
*/
int get_value(const HashState& state, long hash, long size){
    const auto data = state.find(hash);
    if (data != state.end()){
        return data->second;
    }
    else{
        return 0;
    }
}

/*
======================================================
|| void resetWindow()
======================================================
|| window -> window array to reset
|| state -> the state to pull values from to fill window
|| size -> size used when hashing
======================================================
|| updates the values in the window tobe centered
|| at a diffrent position
======================================================
*/
template<typename T>
void resetWindow(T& window, const HashState& state, long hash, long size){
    window[0] = get_value(state, hash - (size*HASH_SCALER) - 1, size);
    window[1] = get_value(state, hash - (size*HASH_SCALER), size);
    window[2] = get_value(state, hash - (size*HASH_SCALER) + 1, size);
    window[3] = get_value(state, hash - 1, size);
    window[4] = get_value(state, hash, size);
    window[5] = get_value(state, hash + 1, size);
    window[6] = get_value(state, hash + (size*HASH_SCALER) - 1, size);
    window[7] = get_value(state, hash + (size*HASH_SCALER), size);
    window[8] = get_value(state, hash + (size*HASH_SCALER) + 1, size);
}

/*
======================================================
|| void drawarray()
======================================================
|| array -> array to draw
|| size -> size of array
======================================================
|| output all th e values in array to terminal
======================================================
*/
template<typename T>
void drawarray(T array, int size){
    for (int i = 0; i < size; i++){
        std::cout << array[i];
        if (((i+1)%((int)std::ceil(sqrt(size)))) == 0){
            std::cout << '\n';
        }
    }
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
void evolve(HashState& state1, HashState& state2, long size, RuleSet& ruleSet){
    omp_lock_t writelock;
    omp_init_lock(&writelock);

    std::vector<long> keys;
    for (const auto& [key, value] : state1){
        keys.push_back(key);
    }
    #pragma omp parallel for collapse(3)
    for (int k = 0; k < keys.size() - 1; k++){
        for (int i = -1; i < 2; i++){
            for (int j = -1; j < 2; j++){
                auto key = keys[k];
                int window[WINDOWSIZE];
                resetWindow(window, state1, key + ((size*HASH_SCALER)*i) + j, size);
                const auto weight = calculateWeight(window);
                const auto output = testWeight(weight, window[4], ruleSet);
                if (output != 0){
                    omp_set_lock(&writelock);
                    state2[key + ((size*HASH_SCALER)*i) + j] = output;
                    omp_unset_lock(&writelock);
                }
            }
        }
    }
    omp_destroy_lock(&writelock);

    std::swap(state1, state2);
    state2.clear();
    return;
}

#endif