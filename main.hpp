/*
======================================================
||               Ceullular Automata
======================================================
|| Jamie Westerhout 47451933
|| COSC3500 Sem2 2023
======================================================
*/
#ifndef MAIN_H

#define MAIN_H

#define RENDER 0 //0 off, 1 on
#define FRAMES 100 //number of times to output
#define BENCHMODE 0 //0 off, 1 on: disables file output

#include <unistd.h>
#include <iostream>
#include <string>
#include <math.h>
#include <vector>
#include <unordered_map>
#include <ncurses.h>
#include <chrono>
#include <string.h>
#include "life.hpp"
#include "export.hpp"
#include "import.hpp"

/*
======================================================
|| void render()
======================================================
|| * state1 -> data strucutre that stores the current world state
|| * size -> size of array for array mode or the number to use fo hashing for hash
======================================================
|| renders the current state of the world
|| to the terminal using ncursers
|| ** kinda slow and bad might sawp for opengl display at somepoint
======================================================
*/
template<typename T>
int render(T& state, unsigned size){
    //newterm(getenv("TERM"), stdout, stdin);
    initscr(); //setup ncursers
    cbreak();
    noecho();
    unsigned ogsize = size;
    if (size > 500){
        size = 500; //sets max size :: cant draw anymroe then this to my terminal
    }
    WINDOW* win = newwin(size, size*2, 0, 0);
    for (int y = 0; y < size; y++){ //draw world
        for (int x = 0; x < size * 2; x+=2){
            if (get_value(state,y,x/2,ogsize) == 0){
                mvaddch(y, x, ' ');
                mvaddch(y, x+1, ' ');
            }
            else {
                mvaddch(y, x, ' ' | A_REVERSE);
                mvaddch(y, x+1, ' ' | A_REVERSE);
            }
        
        }
    }
    refresh(); //refresh display
    usleep(50000); //wait between frames
    return 0;
}


/*
======================================================
|| void compute()
======================================================
|| * state1 -> data strucutre that stores the current world state
|| * state2 -> data structed to store the future world sate
|| * size -> size of array for array mode or the number to use fo hashing for hash
|| * rules -> ruleset to be used
|| * generations -> number of generations to simulate
======================================================
|| loops through and calculutes each generation
|| and outputs results
|| outputs 10 generations even between total number of
|\ generations
======================================================
*/
template<typename T>
void compute(T& state1, T& state2, unsigned size, RuleSet& rules, unsigned generations){
    std::chrono::time_point<std::chrono::system_clock> start, end; //create clock

    end = std::chrono::system_clock::now();
    std::time_t end_time = std::chrono::system_clock::to_time_t(end);
#if BENCHMODE == 0
    std::cout << "started saving intial state at " << std::ctime(&end_time);
    exportState(state1); //output state to file
    end = std::chrono::system_clock::now();
    end_time = std::chrono::system_clock::to_time_t(end);
    std::cout << "finished saving intial state at " << std::ctime(&end_time);
    std::cout<<"======================================================\n";
#endif
    start = std::chrono::system_clock::now();

    end = std::chrono::system_clock::now(); //output time of start
    std::chrono::duration<double> elapsed_seconds = end - start;
    end_time = std::chrono::system_clock::to_time_t(end);
    std::cout << "started computation at " << std::ctime(&end_time);
    if (generations < FRAMES){ //ensure number of genrations is higher then number of frames
        std::cout << "number of generations to small, generations increased to " << FRAMES << "\n";
        generations = FRAMES;
    }
    std::cout<<"======================================================\n";
#if CUDA
    cuda_evo_all(state1, state2, size, generations); //main evolve loop is moved to CU file so just one function
#else
    for (int i = 0; i < generations; i++){ //loop through generations
#if RENDER == 1 //render state
        render(state1, size);
#else //ese do terminal output version
        if (i%(generations/FRAMES) == 0 && i != 0){
            end = std::chrono::system_clock::now();
            std::chrono::duration<double> elapsed_seconds = end - start; //output time after computing 10% of calcualtions
            std::cout << "| (" << i << "/" << generations << "):   " << "elapsed time: " << elapsed_seconds.count() << "s\n";
            //exportState(state1); //export to file
        }
#endif
        evolve(state1, state2, size, rules); //calculate next generatpion
    }
#endif
    end = std::chrono::system_clock::now();
    elapsed_seconds = end - start;
    end_time = std::chrono::system_clock::to_time_t(end);
    std::cout<<"======================================================\n"; //ouput end time
    std::cout << "finished computation at " << std::ctime(&end_time)
              << "total elapsed time: " << elapsed_seconds.count() << "s\n";
    std::cout<<"======================================================\n";
#if BENCHMODE == 0
    std::cout << "started saving output at " << std::ctime(&end_time);
    exportState(state1); //output state to file
    end = std::chrono::system_clock::now();
    end_time = std::chrono::system_clock::to_time_t(end);
    std::cout << "finished saving output at " << std::ctime(&end_time);
    std::cout<<"======================================================\n";
#endif
}

/*
======================================================
|| void start()
======================================================
|| * mode -> data strcture to store world in (1=array, 2=hash)
|| * size -> size of array for array mode or the number to use fo hashing for hash
|| * ruleSet -> ruleset to use
|| * generations -> number of generations to simulate
|| * filename -> location of file to use for intial world state
======================================================
|| creates the data structures to store the state of
|| the world and importing the intial state from file,
|| once created start simulating the world
======================================================
*/
void start(int mode, unsigned size, RuleSet& ruleSet, unsigned generations, std::string filename) {
#if CUDA == 0
    if (mode == 1){ //array mode: world is stored in std::vector's
#endif
        VectorState state1; // create two states, 1 hodl current world state
        VectorState state2; // 2 holds gets fildledfrom the state of 1

        state1.resize(size);  
        state2.resize(size);

        for (int i = 0; i < size; i++){
            state1[i].resize(size);
            state2[i].resize(size);
        }

        state1 = importVectorState(filename, size);

        compute(state1, state2, size, ruleSet, generations);
#if CUDA == 0
    }
    else if (mode == 2){ //hash mode: world is stored in unordered maps
        HashState state1; // create two states, 1 hodl current world state
        HashState state2; // 2 holds gets fildledfrom the state of 1

        state1 = importArrayStateAsHashState(filename, size);
        compute(state1, state2, size, ruleSet, generations);
    }
#endif
}

#endif