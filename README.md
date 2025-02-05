# Cellular Automata HPC
Customisable Cellular Automata written in C++ using OpenMP, MPI and CUDA used to compare the diffreneces in performance between different parallelisation methods

## Introduction to Cellular Automata 
this is a cellular automata with a changeable ruleset. the most well know cellular automata "the game of life" has the rules that if there are 2 or 3 adjacent cells alive then the cell stays live and if there a 3 adjacent cells to a dead cells then it comes alive. But my implementation allows the input of any number of cells for the survive and birth conditions

## Overview
- The state of the world is stored in 2D C++ std vector.
- Every generation loop through all cells in vector and calculates number of alive neighbours which is then used to determine whether the cells should be dead or alive next generations, the results is then written to a second Vector.
- Once all cells are check the original vector is swapped with the second vector now which now contains the next generation. Repeat until all generations are complete.
- However problems can occur when live cells get close to the edge of the Vector so, If a check goes over the edge of the world (end of vector), it wraps around to the other side



### OpenMP (Multi-Threading)
Cell checks are split across threads using #pragma omp parallel for collapse(2) on the evolve for loop
![image](https://github.com/SixLeopard/Cellular_Automata_HPC/blob/main/Images/Slide15.PNG)

### MPI (Multi-Node)
Cell checks split across Nodes, then also uses the same OpenMP multithreading on the nodes from before
![image](https://github.com/SixLeopard/Cellular_Automata_HPC/blob/main/Images/Slide18.PNG)
![image](https://github.com/SixLeopard/Cellular_Automata_HPC/blob/main/Images/Slide19.PNG)
![image](https://github.com/SixLeopard/Cellular_Automata_HPC/blob/main/Images/Slide20.PNG)
![image](https://github.com/SixLeopard/Cellular_Automata_HPC/blob/main/Images/Slide21.PNG)
![image](https://github.com/SixLeopard/Cellular_Automata_HPC/blob/main/Images/Slide22.PNG)
![image](https://github.com/SixLeopard/Cellular_Automata_HPC/blob/main/Images/Slide23.PNG)
![image](https://github.com/SixLeopard/Cellular_Automata_HPC/blob/main/Images/Slide24.PNG)

### Cuda
Uses the GPU, since the most GPUs have  thousands of cores instead of just a few like CPUs have it can be massively parallelised.
![image](https://github.com/SixLeopard/Cellular_Automata_HPC/blob/main/Images/Slide30.PNG)
![image](https://github.com/SixLeopard/Cellular_Automata_HPC/blob/main/Images/Slide31.PNG)
![image](https://github.com/SixLeopard/Cellular_Automata_HPC/blob/main/Images/Slide32.PNG)
![image](https://github.com/SixLeopard/Cellular_Automata_HPC/blob/main/Images/Slide33.PNG)

## Results
- All Bench marks were run by running the program on for all the test cases on the right for 100 generations.
- The results taking from them were only the time taking to complete the 100 generations. This includes any additional time taking to copy memory or communication apart of a particular parallelisation method but doesn't included time taken to import or export data
- in cases where multiple nodes finish at separate times the longest time was always take
### Test Files
| Tests | Test File                   | Cell count  |
| ----- | --------------------------- | ----------- |
| 1     | universalturingmachine.life | 400,000,000 |
| 2     | turingmachine.life          | 100,000,000 |
| 3     | turingmachine.life          | 25,000,000  |
| 4     | sawtooth.life               | 1,000,000   |
| 5     | ArrayGlider-100.life        | 562,500     |
| 6     | ArrayGlider-100.life        | 250,000     |
| 7     | ArrayGlider-100.life        | 625,00      |
| 8     | ArrayGlider-100.life        | 100         |

### Overall Results
| Cells  | 400000000 | 100000000 | 25000000 | 1000000  | 562500   | 250000   | 62500    | 100      |
| ------ | --------- | --------- | -------- | -------- | -------- | -------- | -------- | -------- |
| Serial | 138.511s   | 34.6999s   | 8.8182s   | 0.350179s | 0.200059s | 0.08722s  | 0.022642s | 0.000261s |
| OMP    | 43.9713s   | 11.1775s   | 3.09118s  | 0.114765s | 0.063866s | 0.047792s | 0.007799s | 0.000679s |
| MPI    | 19853.1s   | 3622.52s   | 701.825s  | 13.4531s  | 6.55774s  | 3.15742s  | 1.06508s  | 0.085449s |
| CUDA   | 3.28898s   | 0.700402s   | 0.220769s | 0.0779065s | 0.0782972s | 0.0751151s | 0.0730136s | 0.0720081s |

### Induvidual Results
![image](https://github.com/SixLeopard/Cellular_Automata_HPC/blob/main/Images/Slide16.PNG)
![image](https://github.com/SixLeopard/Cellular_Automata_HPC/blob/main/Images/Slide25.PNG)
![image](https://github.com/SixLeopard/Cellular_Automata_HPC/blob/main/Images/Slide26.PNG)
![image](https://github.com/SixLeopard/Cellular_Automata_HPC/blob/main/Images/Slide34.PNG)

### Comparisons
![image](https://github.com/SixLeopard/Cellular_Automata_HPC/blob/main/Images/Slide36.PNG)
![image](https://github.com/SixLeopard/Cellular_Automata_HPC/blob/main/Images/Slide37.PNG)
![image](https://github.com/SixLeopard/Cellular_Automata_HPC/blob/main/Images/Slide38.PNG)
![image](https://github.com/SixLeopard/Cellular_Automata_HPC/blob/main/Images/Slide39.PNG)

### Review
- Overall CUDA solution is the best as even though it lost in smaller tests it still only took roughly 0.07 seconds, meaning that its significantly better where the time to complete becomes significant
- OpenMP being the best without using a GPU and MPI preforming fairly bad for this specific case

## Possible Future Imporvements
- storing the state in a run length encoded format to see if the added overhead of having to deal with the encoding would be faster then the time it takes to check redundant cells

# Credits
- Author: Jamie Westerhout
- OpenMP: https://www.openmp.org/
- MPI: https://www.open-mpi.org/
- CUDA: https://developer.nvidia.com/cuda-toolkit

#Note
this is recreation of the orignal repositry
