/*
======================================================
||               Ceullular Automata
======================================================
|| Jamie Westerhout 47451933
|| COSC3500 Sem2 2023
======================================================
*/
#ifndef IMPORT_H

#define IMPORT_H

#include "life.hpp"
#include <fstream>
#include <sstream>

/*
======================================================
|| void importVectorState()
======================================================
|| filename -> locaton of file contating a world state in vectore state format
|| size -> the size of array the state is to be written into
======================================================
|| returns a vector state containg the state from file at filename
======================================================
*/
VectorState importVectorState(std::string filename, unsigned size){
    std::ifstream infile(filename); //create stream to file
    std::string line; //string to store each line

    VectorState out; //vectore state to return
    out.resize(size); //resize to desired size of array

    int count = 0;
    while (count < size){
        getline (infile, line); //get new line
        int count2 = 0;
        for (char& c : line){
            if (count2 >= size){ //if current lenth is greater then desired size of array contiune
                continue;
            }
            unsigned val = c - '0'; //turn value into unsigned into
            out[count].push_back(val); //push value to array
            count2++;
        }
        while (count2 < size) //after completing line if its still smalle rthen array
        { //fill with zeros until its full
            out[count].push_back(0);
            count2++;
        }
        count += 1;
    }
    infile.close(); //close file

    return out;
}

/*
======================================================
|| void importHashState()
======================================================
|| filename -> locaton of file contating a world state in hash state format
|| size -> the size to uses for hashing
======================================================
|| returns a hash state containg the state from file at filename
======================================================
*/
HashState importHashState(std::string filename, unsigned size){
    std::ifstream infile(filename); //create steam for file
    std::string line;

    HashState out; //hashstate to be output
#if CUDA == 0
    while (getline(infile, line)){
        std::stringstream ss(line); //read line
        while (getline(ss, line, '|')){ //split on |
            const long hash = std::stol(line); //turn contents into number
            std::pair<int, int> pos = posUnHash(hash, size); //unhash
            out[posHash(pos.first, pos.second, size)] = 1; //rehash and add to hashtable with value 1
        }
    }
    infile.close(); //close file
#endif
    return out; //return created hashstate
}

/*
======================================================
|| void importArrayStateAsHashState()
======================================================
|| filename -> locaton of file contating a world state in vector state format
|| size -> the size to uses for hashing
======================================================
|| returns a hash state containg the state from file at filename
======================================================
*/
HashState importArrayStateAsHashState(std::string filename, unsigned size){
    std::ifstream infile(filename); //create filestream for filename
    std::string line;

    HashState out; //hashsate to ouput
#if CUDA == 0
    int count = 0;
    while (getline (infile, line)){ //read lines
        int count2 = 0;
        for (char& c : line){
            unsigned val = c - '0'; //convet to unsigned int
            if (val == 1){
                out[posHash(count, count2, size)] = val; //add value to hashstate
            }
            count2++;
        }
        count += 1;
    }
    infile.close(); //close file
#endif
    return out; //return new hashstate
}


#endif