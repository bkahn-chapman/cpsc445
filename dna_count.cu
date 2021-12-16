#include <iostream>
#include <vector>
#include <fstream>
#include <stdio.h>
#include <cmath>
#include <array>
using namespace std;

__global__
void count(int *b, int *c, int N) {
    __syncthreads();
    int i = blockIdx.x;
    __syncthreads();
    if (i<N) {
        int t = b[i];
        __syncthreads();
        c[t]++; 
        __syncthreads();
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
    int hb[N];
    int hc[4];
    int *db;
    int *dc;
    cudaMalloc((void **)&db, N*sizeof(int));
    cudaMalloc((void **)&dc, 4*sizeof(int));
    for (int i = 0; i<N; ++i) {
        ha[i] = dna[i];
    }
    for(int i = 0; i<N; ++i) {
        if(ha[i] == 'A')
        {
            hb[i] = 0;
        }
        if(ha[i] == 'T')
        {
            hb[i] = 1;
        }
        if(ha[i] == 'G')
        {
            hb[i] = 2;
        }
        if(ha[i] == 'C')
        {
            hb[i] = 3;
        }
    }
    for(int i = 0; i < 4; ++i)
    {
        hc[i] = 0;
    }
    cudaMemcpy(db, hb, N*sizeof(int), cudaMemcpyHostToDevice);
    count<<<N, 1>>>(db, dc, N);
    cudaMemcpy(hc, dc, 4*sizeof(int), cudaMemcpyDeviceToHost);
    ofstream outFS;
    outFS.open("output.txt");
    outFS << "A " << hc[0] << endl;
    outFS << "T " << hc[1] << endl;
    outFS << "G " << hc[2] << endl;
    outFS << "C " << hc[3] << endl;
    outFS.close();
    cudaFree(db);
    cudaFree(dc);
    return 0;
}
