#include <iostream>
#include <vector>
#include <fstream>
#include <stdio.h>
#include <cmath>
#include <array>
using namespace std;

__global__
void count(char *a, char *b, int N) {
    int i = blockIdx.x;
    if (i<N) {
        if(a[i] == 'A')
        {
            b[i] = '1';
        }
        if(a[i] == 'C')
        {
            b[i] = '2';
        }
        if(a[i] == 'G')
        {
            b[i] = '3';
        }
        if(a[i] == 'T')
        {
            b[i] = '4';
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
    char ha[N], hb[N];
    char *da, *db;
    cudaMalloc((void **)&da, N*sizeof(char));
    for(int i = 0; i<N; ++i) {
        ha[i] = dna[i];
    }
    cudaMemcpy(da, ha, N*sizeof(char), cudaMemcpyHostToDevice);
    cudaMemcpy(db, hb, N*sizeof(char), cudaMemcpyHostToDevice);
    count<<<N, 1>>>(da, db, N);
    cudaMemcpy(hb, db, N*sizeof(char), cudaMemcpyDeviceToHost);
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