/*
======================================================
||               Ceullular Automata
======================================================
|| Jamie Westerhout 47451933
|| COSC3500 Sem2 2023
======================================================
*/
#ifndef LIFE_CORE_H

#define LIFE_CORE_H

#include "life.hpp"

/*
======================================================
|| void calculateWeight()
======================================================
|| window -> 3x3 of cells from the world
======================================================
|| add upp all the states of the  cells in the
|| window except for the middle one
======================================================
*/
template<typename T>
unsigned calculateWeight(const T& window){
    unsigned weight = 0;
    for (int i = 0; i < WINDOWSIZE; i++){
        weight += window[i];
    }
    weight -= window[4];
    return weight;
}

/*
======================================================
|| void testWeight()
======================================================
|| weight -> weight calculted from calculate window
|| current_status -> the value in the current cell being tested
|| rules -> ruleset to tes against
======================================================
|| return wether the cell all current status
|| shouild die, birth or stay alive
======================================================
*/
unsigned testWeight(unsigned weight, unsigned current_status, RuleSet& rules){
    if (current_status == 1){
        for (unsigned i : rules.first){
            if (weight == i){
                return 1;
            }
        }
    }
    else if (current_status == 0){
        for (unsigned i : rules.second){
            if (weight == i){
                return 1;
            }
        }
    }
    return 0;
}

#endif