#include <iostream>
#include <vector>
#include <fstream>
#include <stdio.h>
#include <cmath>
#include <array>
using namespace std;

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
    for(int i = 0; i < ha.size(); ++i)
    {
        cout << ha[i];
    }
    cout << endl;
}