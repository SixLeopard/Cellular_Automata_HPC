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
void evolve(VectorState& state1, VectorState& state2, unsigned size, RuleSet& ruleSet){
    #pragma omp parallel for collapse(2)
    for (int y = 1; y < size; y++){
        for (int x = 1; x < size; x++){
            u_int8_t window[WINDOWSIZE];
            resetWindow(window, state1, x, y, size);
            const auto weight = calculateWeight(window);
            state2[y][x] = testWeight(weight, state1[y][x], ruleSet);
        }
    }
    std::swap(state1, state2);
    return;
}

#endif