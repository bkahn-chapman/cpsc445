#include <iostream>
#include <vector>
#include <fstream>
#include <stdio.h>
#include <cmath>
#include <array>
using namespace std;

__global__
void count(char *a, int *b, int N) {
    int stride = blockDim.x * 2;
    int tid = threadIdx.x;
    for(int i=tid+W; i<N; i+=W)
    {
        if (i<N) {
            if(a[i] == 'A')
            {
                b[tid]++;
            }
            if(a[i] == 'C')
            {
                b[tid]++;
            }
            if(a[i] == 'G')
            {
                b[tid]++;
            }
            if(a[i] == 'T')
            {
                b[tid]++;
            }
        }
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
    char *da;
    int *hb = new int[4];
    int *db;
    cudaMalloc((void **)&da, N*sizeof(char));
    cudaMalloc((void **)&db, N*sizeof(int));
    for (int i = 0; i<N; ++i) {
        ha[i] = dna[i];
    }
    cudaMemcpy(da, ha, N*sizeof(char), cudaMemcpyHostToDevice);
    cudaMemcpy(db, hb, N*sizeof(int), cudaMemcpyHostToDevice);
    int W = 4;
    count<<<1,W>>>(da, db, N);
    cudaDeviceSynchronize();
    int sums[4];
    cudaMemcpy(sums, da, W*sizeof(int), cudaMemcpyDeviceToHost);
    for(int i = 0; i < 4; ++i)
    {
        cout << sums[i] << endl;
    }
}