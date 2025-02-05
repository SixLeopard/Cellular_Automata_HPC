/*
======================================================
||               Ceullular Automata
======================================================
|| Jamie Westerhout 47451933
|| COSC3500 Sem2 2023
======================================================
*/
#ifndef LIFE_H

#define LIFE_H

#include <unistd.h>
#include <iostream>
#include <string>
#include <math.h>
#include <vector>
#include <unordered_map>
#include <ncurses.h>
#include <chrono>
#include <string.h>
#include <omp.h>

//only array mode is fully functional when compiling with either CUDA or MPI enabled
//CUDA also only supports standard life ruleset
#define CUDA 1 //0 off, 1 on, make sure to uncomment line in CMakeLists.txt before compiling with CUDA enabled
#define ENABLE_MPI 0 //0 off, 1 on can not be one aswell as cuda

//data strcutre fro array mode 
typedef std::vector<std::vector<unsigned>> VectorState;
//data strcutre for hash mode
typedef std::unordered_map<long, unsigned> HashState;
//data strcutre to store the rulese
typedef std::pair<std::vector<unsigned>, std::vector<unsigned>> RuleSet;
//window size (do not change)
#define WINDOWSIZE 9

#if ENABLE_MPI == 1
    #include "MPIarraylife.hpp"
    #include "hashlife.hpp"
#elif CUDA == 1
    #include "CUDAgeneration.cu"
#else
    #include "arraylife.hpp"
    #include "hashlife.hpp"
#endif
#endif