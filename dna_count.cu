#include <iostream>
#include <vector>
#include <fstream>
#include <stdio.h>
#include <cmath>
#include <array>
using namespace std;

__global__
void count(char *a, int *b, int N) {
    int i = blockIdx.x;
    if (i<N) {
        if(a[i] == 'A')
        {
            b[0]++;
        }
        if(a[i] == 'C')
        {
            b[1]++;
        }
        if(a[i] == 'G')
        {
            b[2]++;
        }
        if(a[i] == 'T')
        {
            b[3]++;
        }
    }
}

int main () {
    ifstream inFS;
    inFS.open("dna.txt");
    string dna;
    getline(inFS, dna);
    int N = dna.length();
    char ha[N];
    int hb[4];
    char *da;
    int *db;
    cudaMalloc((void **)&da, N*sizeof(char));
    cudaMalloc((void **)&db, 4*sizeof(int));
    for (int i = 0; i<N; ++i) {
        ha[i] = dna[i];
    }
    for (int i = 0; i < 4; ++i) {
        hb[i] = 0;
    }
    cudaMemcpy(da, ha, N*sizeof(char), cudaMemcpyHostToDevice);
    count<<<N, 1>>>(da, db, N);
    cudaMemcpy(hb, db, 4*sizeof(int), cudaMemcpyDeviceToHost);
    ofstream outFS;
    outFS.open("output.txt");
    char letters[] = {'A', 'C', 'G', 'T'};
    for(int i = 0; i<4; ++i)
    {
      outFS << letters[i] <<  " " << hb[i];
    }
    outFS.close();
    cudaFree(da);
    cudaFree(db);
    return 0;
}