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
            b[i] = 'A';
        }
        if(a[i] == 'C')
        {
            b[i] = 'C';
        }
        if(a[i] == 'G')
        {
            b[i] = 'G';
        }
        if(a[i] == 'T')
        {
            b[i] = 'T';
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
    cudaMalloc((void **)&db, N*sizeof(char));
    for (int i = 0; i<N; ++i) {
        ha[i] = dna[i];
    }
    cudaMemcpy(da, ha, N*sizeof(char), cudaMemcpyHostToDevice);
    count<<<N, 1>>>(da, db, N);
    cudaMemcpy(hb, db, N*sizeof(char), cudaMemcpyDeviceToHost);
    ofstream outFS;
    outFS.open("output.txt");
    int aCnt = 0;
    int cCnt = 0;
    int gCnt = 0;
    int tCnt = 0;
    for(int i = 0; i<N; ++i)
    {
        if(hb[i] == 'A')
        {
            aCnt++;
        }
        if(hb[i] == 'C')
        {
            cCnt++;
        }
        if(hb[i] == 'G')
        {
            gCnt++;
        }
        if(hb[i] == 'T')
        {
            tCnt++;
        }
    }
    outFS << "A " << aCnt << endl;
    outFS << "T " << tCnt << endl;
    outFS << "G " << gCnt << endl;
    outFS << "C " << cCnt << endl;
    outFS.close();
    cudaFree(da);
    cudaFree(db);
    return 0;
}
