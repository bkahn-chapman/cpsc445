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
    int test[4];
    for(int c = 0; c < 4; ++c)
    {
        test[c] = 0;
    }
    if (i<N) {
        if(a[i] == 'A')
        {
            test[0]++;
        }
        if(a[i] == 'C')
        {
            test[1]++;
        }
        if(a[i] == 'G')
        {
            test[2]++;
        }
        if(a[i] == 'T')
        {
            test[3]++;
        }
    }
    __syncthreads();
    cout << test[0] << " " << test[1] << " " << test[2] << " " << test[3] << endl;
    for(int t = 0; t < 4; ++t)
    {
        b[t] = share[t];
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
      outFS << hb[i];
    }
    outFS.close();
    cudaFree(da);
    cudaFree(db);
    return 0;
}