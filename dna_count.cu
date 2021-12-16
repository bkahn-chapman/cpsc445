#include <iostream>
#include <vector>
#include <fstream>
#include <stdio.h>
#include <cmath>
#include <array>
using namespace std;

extern __shared__ int results[4];

__global__
void count(char *a, int *b, int N) {
    int i = blockIdx.x;
    int test[4];
    for(int i = 0; i < 4; ++i)
    {
        test[i] = 0;
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
    char *da;
    int *db;
    cudaMalloc((void **)&da, N*sizeof(char));
    cudaMalloc((void **)&db, N*sizeof(int));
    for(int i = 0; i<N; ++i) {
        ha[i] = dna[i];
        hb[i] = 0;
    }
    cudaMemcpy(da, ha, N*sizeof(char), cudaMemcpyHostToDevice);
    count<<<N, 1>>>(da, db, N);
    cudaMemcpy(hb, db, N*sizeof(int), cudaMemcpyDeviceToHost);
    ofstream outFS;
    outFS.open("output.txt");
    for(int i = 0; i<N; ++i)
    {
      outFS << db[i];
    }
    outFS.close();
    cudaFree(da);
    cudaFree(db);
    return 0;
}