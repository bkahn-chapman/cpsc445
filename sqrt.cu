#include <iostream>
#include <vector>
#include <fstream>
#include <stdio.h>
#include <cmath>
#include <array>
#include <string>
using namespace std;

__global__
void sqrt(double *a, double *b, int N) {
    int i = blockIdx.x;
    if (i<N) {
        b[i] = sqrt(a[i]);
    }
}

int main () {
    ifstream inFS;
    inFS.open("input.csv");
    // system("head input.csv");
    vector<double> nums;
    string line;
    while(getline(inFS, line))
    {
        string num = "";
        for(char &c : line)
        {
            if(c == '\n')
            {
                nums.push_back(stod(num));
            }  
            else
            {
                num.push_back(c);
            }
        }
    }
    int N = nums.size();
    double ha[N], hb[N];
    double *da, *db;
    cudaMalloc((void **)&da, N*sizeof(double));
    cudaMalloc((void **)&db, N*sizeof(double));
    for (int i = 0; i<N; ++i) {
        ha[i] = nums[i];
    }
    cudaMemcpy(da, ha, N*sizeof(char), cudaMemcpyHostToDevice);
    invert<<<N, 1>>>(da, db, N);
    cudaMemcpy(hb, db, N*sizeof(char), cudaMemcpyDeviceToHost);
    ofstream outFS;
    outFS.open("output.txt");
    for(int i = 0; i<N; ++i)
    {
      outFS << hb[i];
    }
    outFS.close();
    cudaFree(da);
    cudaFree(db);
    return 0;
}