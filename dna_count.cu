#include <iostream>
#include <vector>
#include <fstream>
#include <stdio.h>
#include <cmath>
#include <array>
using namespace std;

__global__
void invert(char *a, char *b, int N) {
    int i = blockIdx.x;
    if (i<N) {
        if(a[i] == 'A')
        {
            b[i] = '0';
        }
        if(a[i] == 'C')
        {
            b[i] = '1';
        }
        if(a[i] == 'G')
        {
            b[i] = '2';
        }
        if(a[i] == 'T')
        {
            b[i] = '3';
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
    invert<<<N, 1>>>(da, db, N);
    cudaMemcpy(hb, db, N*sizeof(char), cudaMemcpyDeviceToHost);
    ofstream outFS;
    outFS.open("output.txt");
    int aCnt;
    int cCnt;
    int gCnt;
    int tCnt;
    for(int i = 0; i<N; ++i)
    {
        if(hb[i] == '0')
        {
            aCnt++;
        }
        if(hb[i] == '1')
        {
            cCnt++;
        }
        if(hb[i] == '2')
        {
            gCnt++;
        }
        if(hb[i] == '3')
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
