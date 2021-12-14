#include <iostream>
#include <vector>
#include <fstream>
#include <stdio.h>
#include <cmath>
#include <array>
using namespace std;

//test

__global__
void count(char *a, int *b, int N) {
    int i = blockIdx.x;
    __shared__ int share[4];
    if (i<N) {
        if(a[i] == 'A')
        {
            share[0]++;
        }
        if(a[i] == 'C')
        {
            share[1]++;
        }
        if(a[i] == 'G')
        {
            share[2]++;
        }
        if(a[i] == 'T')
        {
            share[3]++;
        }
    }
    __syncthreads();
    for(int t = 0; t < 4; ++t)
    {
        b[t] = shared[t];
    }
}

int main () {
    ifstream inFS;
    inFS.open("dna.txt");
    string dna;
    getline(inFS, dna);
    inFS.close();
    int N = dna.length();
    char ha[N];
    int hb[4];
    char *da;
    int *db;
    cudaMalloc((void **)&da, N*sizeof(char));
    cudaMalloc((void **)&db, N*sizeof(int));
    for (int i = 0; i<N; ++i) {
        ha[i] = dna[i];
    }
    cudaMemcpy(da, ha, N*sizeof(char), cudaMemcpyHostToDevice);
    count<<<N, 1>>>(da, db, N);
    cudaMemcpy(hb, db, N*sizeof(int), cudaMemcpyDeviceToHost);
    ofstream outFS;
    outFS.open("output.txt");
    for(int i = 0; i<4; ++i)
    {
      outFS << share[i];
    }
    outFS.close();
    cudaFree(da);
    cudaFree(db);
    return 0;
}