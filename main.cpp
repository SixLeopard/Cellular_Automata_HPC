/*
======================================================
||               Ceullular Automata
======================================================
|| Jamie Westerhout 47451933
|| COSC3500 Sem2 2023
======================================================
*/
#include "main.hpp"
#include <limits>

int main(int argc, char* argv[]){
#if ENABLE_MPI
    MPI_Init(&argc, &argv);
#endif
    //output heading
    std::cout<<"======================================================\n";
    std::cout<<"||                 Cellular Automata                ||\n";
    std::cout<<"======================================================\n";

    //cmd arg processing
    int mode = 2;
    unsigned size = 50;
    unsigned generations = 200;
    int file_given = 0;
    std::string survivecons = "23";
    std::string birthcons = "3";
    std::string filelocation = "None Given";
    for (int i = 0; i < argc-1; i++){
        if (strcmp(argv[i], "-m") == 0){ //mode
            if (strcmp(argv[i+1], "array") == 0){
                mode = 1;
            }
            else if (strcmp(argv[i+1], "hash") == 0)
            {
                mode = 2;
            }
            else{
                std::cout<<"Incorrect CMDargs: [-m [hash,array]] [-s [n1 n1 n3 ...]] [-b [n1 n2 n3 ...]] [-f [gof filename]] [-n size] [-g generations]\n";
            }
        }
        else if (strcmp(argv[i], "-s") == 0){ //survival rules
            survivecons = argv[i+1];
        }
        else if (strcmp(argv[i], "-b") == 0){ //birth rules
            birthcons = argv[i+1];
        }
        else if (strcmp(argv[i], "-f") == 0){ //file input location
            filelocation = argv[i+1];
            file_given = 1;
        }
        else if (strcmp(argv[i], "-n") == 0){ //size of world for array mode
            size = std::stoi(argv[i+1]);
        }
        else if (strcmp(argv[i], "-g") == 0){ //number of generations to simulate
            generations = std::stoi(argv[i+1]);
        }
    }

    //store string versions of the ruleset
    std::string ruleSet[2] = {survivecons, birthcons};

    std::vector<unsigned> survive;
    std::vector<unsigned> birth;
    //convert string verstion into vector form
    for (char& c : ruleSet[0]){
        survive.push_back(c - '0');
    }
    for (char& c : ruleSet[1]){
        birth.push_back(c - '0');
    }
    //store vectore forms of rules in pair
    RuleSet rules = std::make_pair(survive, birth);

    //display chosen options
    std::cout<<"|| Mode: " << mode << "                  (1 -> array, 2 -> hash) \n";
    if (mode == 2) {
        size = std::numeric_limits<unsigned>::max();
        if (file_given == 0){
            filelocation = "resources/Hash-Glider-100.life";
        }
        std::cout<<"|| Generations: " << generations << "\n";
    }
    else if (mode == 1){
        if (file_given == 0){
            filelocation = "resources/ArrayGlider-100.life";
        }
        std::cout<<"|| Array Size: " << size << "\n";
        std::cout<<"|| Generations: " << generations << "\n";
    }
    std::cout<<"|| Survival Conditions: " << survivecons << "\n";
    std::cout<<"|| Birth Conditions: " << birthcons << "\n";
    std::cout<<"|| File Location: " << filelocation << "\n";
    std::cout<<"======================================================\n";

    start(mode, size, rules, generations, filelocation); //start actual program with input cmd args

    endwin(); //end ncuresers window if it was used
    return 0;
}