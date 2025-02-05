# Cellular_Automata_HPC
Customisable Cellular Automata written in C++ using OpenMP, MPI and CUDA used to compare the diffreneces in performance between different parallelisation methods

## Performance:
| Cells  | 400000000 | 100000000 | 25000000 | 1000000  | 562500   | 250000   | 62500    | 100      |
| ------ | --------- | --------- | -------- | -------- | -------- | -------- | -------- | -------- |
| Serial | 138.511s   | 34.6999s   | 8.8182s   | 0.350179s | 0.200059s | 0.08722s  | 0.022642s | 0.000261s |
| OMP    | 43.9713s   | 11.1775s   | 3.09118s  | 0.114765s | 0.063866s | 0.047792s | 0.007799s | 0.000679s |
| MPI    | 19853.1s   | 3622.52s   | 701.825s  | 13.4531s  | 6.55774s  | 3.15742s  | 1.06508s  | 0.085449s |
| CUDA   | 6.51982s   | 1.68107s   | 0.499383s | 0.137289s | 0.130311s | 0.123818s | 0.119705s | 0.117259s |
