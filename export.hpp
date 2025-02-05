/*
======================================================
||               Ceullular Automata
======================================================
|| Jamie Westerhout 47451933
|| COSC3500 Sem2 2023
======================================================
*/
#ifndef EXPORT_H

#define EXPORT_H

#include "life.hpp"
#include <fstream>

/*
======================================================
|| void exportState()
======================================================
|| state -> vectore state to output
======================================================
|| outputs the vectore state to file
======================================================
*/
void exportState(VectorState state){
    auto seconds = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    static std::string filename = "./results/output-" + std::to_string(seconds) + ".life"; //fileouput location
    std::ofstream outfile(filename, std::ios::app); //create file stream to output
    for (std::vector<unsigned>& v : state){
        for (unsigned i : v){ //for each value in the array write it to file
            outfile << i;
        }
        outfile << "\n"; //new line fo rnew line in 2d array
    }
    outfile << "\n"; //ending new line
    outfile.close(); //close file
}

/*
======================================================
|| void exportState()
======================================================
|| state -> hashstate to outpu tto file
======================================================
|| outputs the hasstate to file
======================================================
*/
void exportState(HashState state){
    auto seconds = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    static std::string filename = "./results/output-" + std::to_string(seconds) + ".life"; //fileouput location
    std::ofstream outfile(filename, std::ios::app);
    int count = 0;
    for (auto& [key, value] : state){ //add each key to the file
        count += 1;
        outfile << key << "|";
        if (count%100 == 0) //every 100 add a new line
            outfile << "\n";
    }
    if (count%100 != 0){
        outfile << "\n";
    }
    outfile << "\n"; //final new line
    outfile.close(); //close file
}

#endif