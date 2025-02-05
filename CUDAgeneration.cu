#ifndef CUDA_EVO_ALL

#define CUDA_EVO_ALL

#include "life.hpp"

/*
======================================================
|| void cu_evolve()
======================================================
|| same as normal evolve exepct run on device (GPU)
|| all extranl function used were put into this function
|| to simplify GPU code
======================================================
|| state1 -> state that holds the current world state
|| state2 -> state to use a temproy storgae for future state
|| size -> the size to use for hashing
======================================================
|| returns and updated state1
======================================================
*/
__global__ void cu_evolve(unsigned* state1, unsigned* state2, const unsigned size) {
    //set up where to start and go for this specifc run based on where it is in the GPU thread block grid
    int idx = (blockDim.x*blockDim.y)*threadIdx.z+(threadIdx.y*blockDim.x)+(threadIdx.x); //thread id in block
    int bdx = (gridDim.x*gridDim.y)*blockIdx.z +(blockIdx.y*gridDim.x)+(blockIdx.x); //block id in grid
    int bsize = blockDim.x*blockDim.y*blockDim.z; //block size
    int gsize = gridDim.x*gridDim.y*gridDim.z; //grid size
    //frist one needs to start at 1 since the world as a one space border
    if (bdx == 0) {
        bdx = 1;
    }
    // main update loop
    for (int y = bdx; y < size; y+= gsize){
        for (int x = idx; x < size; x += bsize){
            u_int8_t window[WINDOWSIZE];

            //expanded resetWindow()
            int left;
            int right;
            int up;
            int down;
            if (x == 1) {
                left = size - 1;
                right = x + 1;
            }
            else if (x == size - 1) {
                left = x - 1;
                right = 1;
            }
            else {
                left = x - 1;
                right = x + 1;
            }

            if (y == 1) {
                up = size - 1;
                down = y + 1;
            }
            else if (y == size - 1) {
                up = y - 1;
                down = 1;
            }
            else {
                up = y - 1;
                down = y + 1;
            }
            window[0] = state1[up*size+left];
            window[1] = state1[up*size+x];
            window[2] = state1[up*size+right];
            window[3] = state1[y*size+left];
            window[4] = state1[y*size+x];
            window[5] = state1[y*size+right];
            window[6] = state1[down*size+left];
            window[7] = state1[down*size+x];
            window[8] = state1[down*size+right];
            unsigned weight = 0;
            //end of resetWindow()

            //expanded calculateWeight()
            for (int i = 0; i < WINDOWSIZE; i++){
                weight += window[i];
            }
            int updated = 0;
            weight -= window[4];
            //end of calculateWeight()

            //expanded testWeight() with generalised ruleset
            //removed to simplify arguments that need to be moved ot GPU memory
            if (state1[y*size+x] == 1){
                for (unsigned i : {2,3}){
                    if (weight == i){
                        state2[y*size+x] = 1;
                        updated = 1;
                    }
                }
            }
            else if (state1[y*size+x] == 0){
                for (unsigned i : {3}){
                    if (weight == i){
                        state2[y*size+x] = 1;
                        updated = 1;
                    }
                }
            }
            if (updated == 0) { //add this terms since using C arrays instead of Vectors
                state2[y*size+x] = 0;
            }

            //end of testWeight()
        }
    }
    __syncthreads(); //not really necassary
}

void cuda_evo_all(VectorState& state1, VectorState& state2, unsigned size, unsigned generations) {
    //timer setup
    std::chrono::time_point<std::chrono::system_clock> start, end;
    std::time_t end_time = std::chrono::system_clock::to_time_t(end);

    end = std::chrono::system_clock::now();
    std::cout << "started copying to GPU memory " << std::ctime(&end_time);

    //convert Vector states into 1D C arrays to move to Device
    unsigned* arr_state1 = (unsigned*) malloc(sizeof(unsigned) * size * size);
    unsigned* arr_state2 = (unsigned*) malloc(sizeof(unsigned) * size * size);

    for (unsigned i = 0; i < size; i++) {
        for (unsigned j = 0; j < size; j++) {
            arr_state1[i*size+j] = state1[i][j];
            arr_state2[i*size+j] = state2[i][j];
        }
    }

    //move arrays to device memory from cpu memory
    unsigned* custate1;
    unsigned* custate2;

    cudaMalloc((void**)&custate1, sizeof(unsigned) * size * size);
    cudaMalloc((void**)&custate2, sizeof(unsigned) * size * size);
    cudaMemcpy(custate1, arr_state1, sizeof(unsigned) * size * size, cudaMemcpyHostToDevice);
    cudaMemcpy(custate2, arr_state2, sizeof(unsigned) * size * size, cudaMemcpyHostToDevice);

    //end og mem move timing
    end = std::chrono::system_clock::now();
    end_time = std::chrono::system_clock::to_time_t(end);
    std::cout << "finished copying to GPU memory " << std::ctime(&end_time);
    std::cout<<"======================================================\n";

    start = std::chrono::system_clock::now();

    //normal gen ervolve loop
    for (int i = 0; i < generations; i++){
        //std::cout << "hello " << i << "\n";
        if (i%(generations/FRAMES) == 0 && i != 0){
            end = std::chrono::system_clock::now();
            std::chrono::duration<double> elapsed_seconds = end - start; //output time after computing 10% of calcualtions
            std::cout << "| (" << i << "/" << generations << "):   " << "elapsed time: " << elapsed_seconds.count() << "s\n";
            //exportState(state1); //export to file
        }
        cu_evolve<<<32, 1024>>>(custate1, custate2, size); //run device with 1024x32 grid
        cudaDeviceSynchronize(); //synchronise since previous gens depend on furture gens
        unsigned* temp;
        temp = custate1; //swaps
        custate1 = custate2;
        custate2 = temp;
    }

    std::cout<<"======================================================\n";
    end = std::chrono::system_clock::now();
    std::cout << "started copying from gpu memory to cpu memory " << std::ctime(&end_time);

    //vopy array back of device to cpu so it can be output
    cudaMemcpy(arr_state1, custate1, sizeof(unsigned) * size * size, cudaMemcpyDeviceToHost);

    //convert back to vector
    for (unsigned i = 0; i < size; i++) {
        for (unsigned j = 0; j < size; j++) {
            state1[i][j] = arr_state1[i*size+j];
        }
    }

    //free mem
    free(arr_state1);
    free(arr_state2);
    cudaFree(custate1);
    cudaFree(custate2);

    end = std::chrono::system_clock::now();
    end_time = std::chrono::system_clock::to_time_t(end);
    std::cout << "finished copying from gpu memory to cpu memory " << std::ctime(&end_time);
}

#endif