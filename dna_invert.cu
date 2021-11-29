#include <iostream>
#include <vector>
#include <fstream>
#include <stdio.h>
#include <cmath>
#include <array>
using namespace std;

__global__
void test(char *a, char *b, int N) {
    int i = blockIdx.x;
    if (i<N) {
        if(a[i] == 'A')
        {
            b[i] = 'T';
        }
        if(a[i] == 'C')
        {
            b[i] = 'G';
        }
        if(a[i] == 'G')
        {
            b[i] = 'C';
        }
        if(a[i] == 'T')
        {
            b[i] = 'A';
        }
    }
}

int main () {
    ifstream inFS;
    inFS.open("dna.txt");
    string dna;
    getline(inFS, dna);
    int N = dna.length();
    char ha[N], hb[N];
    char *da, *db;
    cudaMalloc((void **)&da, N*sizeof(char));
    cudaMalloc((void **)&db, N*sizeof(char));
    for (int i = 0; i<N; ++i) {
        ha[i] = dna[i];
    }
    cudaMemcpy(da, ha, N*sizeof(char), cudaMemcpyHostToDevice);
    test<<<N, 1>>>(da, db, N);
    cudaMemcpy(hb, db, N*sizeof(char), cudaMemcpyDeviceToHost);
    ofstream outFS;
    outFS.open("output.txt");
    for(int i = 0; i<N; ++i)
    {
      cout << hb[i];
    }
    outFS.close();
    cudaFree(da);
    cudaFree(db);
    return 0;
}
