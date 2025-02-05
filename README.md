# Cellular_Automata_HPC
Customisable Cellular Automata written in C++ using OpenMP, MPI and CUDA used to compare the diffreneces in performance between different parallelisation methods

## Introduction to Cellular Automata 
this is a cellular automata with a changeable ruleset. the most well know cellular automata "the game of life" has the rules that if there are 2 or 3 adjacent cells alive then the cell stays live and if there a 3 adjacent cells to a dead cells then it comes alive. But my implementation allows the input of any number of cells for the survive and birth conditions

## Optimsations
### OpenMP
The straight forward way to simulate a cellular automata would be to just store the entire world as an array of 1's an 0's, which works well for small worlds that are densely populated. But when the world are sparsely populated this becomes hugely inefficient since you end up storing mostly just 0's in the array and when the worlds get large the memory required to store the array grows rapidly. To optimise this I swapped out the array for a hash table which only stores alive cells because every relevant calculation happens only on or next to alive cells. The hash table uses the position as the key and the value is whether of the cell is alive or not. Look ups in a hash table are mostly in constant time which makes it a reasonable trade off.

### MPI
The way that is was spread across multiple nodes was by splitting the simulation world in chunks that are then spread across nodes. Once a generation has completed calculating on the chunks all the nodes then send the results back to master node to calculate the interactions on the edges of the chunks. after one generation is completed the cycle is repeated

### Cuda
Due the large amount of cores that cpus have 

## Behcnmarking
Benchmarks are run by running a certain number of generations on a particular starting pattern. Time is record and the current state is output every 10% through the simulation and also outputs a final time at the end in seconds. Below are the results found during benchmarking:
### Results:
| Cells  | 400000000 | 100000000 | 25000000 | 1000000  | 562500   | 250000   | 62500    | 100      |
| ------ | --------- | --------- | -------- | -------- | -------- | -------- | -------- | -------- |
| Serial | 138.511s   | 34.6999s   | 8.8182s   | 0.350179s | 0.200059s | 0.08722s  | 0.022642s | 0.000261s |
| OMP    | 43.9713s   | 11.1775s   | 3.09118s  | 0.114765s | 0.063866s | 0.047792s | 0.007799s | 0.000679s |
| MPI    | 19853.1s   | 3622.52s   | 701.825s  | 13.4531s  | 6.55774s  | 3.15742s  | 1.06508s  | 0.085449s |
| CUDA   | 6.51982s   | 1.68107s   | 0.499383s | 0.137289s | 0.130311s | 0.123818s | 0.119705s | 0.117259s |

# Future Imporvements
I would have liked to implement my own hash table class instead of using just using the std::unordered_map. I would also like to implement a way to properly view the output using OpenGL since currently the only way to view it is using the very limited terminal renderer.
