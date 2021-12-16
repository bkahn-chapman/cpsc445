#include <iostream>
#include <vector>
#include <fstream>
#include <stdio.h>
#include <cmath>
#include <array>
#include <string>
#include <algorithm>
using namespace std;

__global__
void parse(int *b, int *c, int N) {
    int i = blockIdx.x;
    if (i<N) {
        int t = b[i];
        c[t]++; //race condition? sometimes?
    }
}

int main () {
    ifstream inFS;
    inFS.open("dna.txt");
    string dna;
    getline(inFS, dna);
    int N = dna.length() / 3;
    string ha[N];
    int hb[N];
    int hc[64];
    int *db;
    int *dc;
    cudaMalloc((void **)&db, N*sizeof(int));
    cudaMalloc((void **)&dc, 64*sizeof(int));
    string triplet;
    for(int i = 0; i < N; ++i)
    {
        triplet.push_back(dna[3*i]);
        triplet.push_back(dna[3*i+1]);
        triplet.push_back(dna[3*i+2]);
        ha[i] = triplet;
        triplet = "";
    }
    char letts[] = {'A', 'C', 'G', 'T'};
    string trips = "";
    for(int t = 0; t < N; ++t)
    {
        int count = 0;
        for(int i = 0; i < 4; ++i)
        {
            for(int j = 0; j < 4; ++j)
            {
                for(int k = 0; k < 4; ++k)
                {
                    trips = "";
                    trips.push_back(letts[i]);
                    trips.push_back(letts[j]);
                    trips.push_back(letts[k]);
                    if(trips == ha[t])
                    {
                        hb[t] = count;
                    }
                    count++;
                }
            }
        }
    }
    for(int i = 0; i < 64; ++i)
    {
        hc[i] = 0;
    }
    cudaMemcpy(db, hb, N*sizeof(int), cudaMemcpyHostToDevice);
    parse<<<N, 1>>>(db, dc, N);
    cudaMemcpy(hc, dc, 64*sizeof(int), cudaMemcpyDeviceToHost);
    ofstream outFS;
    outFS.open("output.txt");
    int count = 0;
    for(int i = 0; i < 4; ++i)
    {
        for(int j = 0; j < 4; ++j)
        {
            for(int k = 0; k < 4; ++k)
            {
                trips = "";
                trips.push_back(letts[i]);
                trips.push_back(letts[j]);
                trips.push_back(letts[k]);
                if(hc[count] > 0)
                {
                    outFS << trips << " " << hc[count] << endl;
                }
                count++;
            }
        }
    }
}