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
            b[i] = 0;
        }
        if(a[i] == 'C')
        {
            b[i] = 1;
        }
        if(a[i] == 'G')
        {
            b[i] = 2;
        }
        if(a[i] == 'T')
        {
            b[i] = 3;
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
    int hb[N];
    char *da;
    int *db;
    cudaMalloc((void **)&da, N*sizeof(char));
    cudaMalloc((void **)&db, N*sizeof(int));
    for(int i = 0; i<N; ++i) {
        ha[i] = dna[i];
    }
    for(int i = 0; i<N; ++i)
    {
        db[i] = 0;
    }
    cout << endl;
    cudaMemcpy(da, ha, N*sizeof(char), cudaMemcpyHostToDevice);
    count<<<N, 1>>>(da, db, N);
    cudaMemcpy(hb, db, N*sizeof(int), cudaMemcpyDeviceToHost);
    ofstream outFS;
    outFS.open("output.txt");
    for(int i = 0; i<hb.size(); ++i)
    {
      outFS << hb[i];
    }
    outFS.close();
    cudaFree(da);
    cudaFree(db);
    return 0;
}