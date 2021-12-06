#include <iostream>
#include <vector>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <cmath>
#include <array>
#include <string>
using namespace std;

__global__
void squareroot(double *a, int N) {
    __shared__ double hb[N];
    int i = blockIdx.x;
    if (i<N) {
        hb[i] = sqrt(a[i]);
    }
}

int main () {
    ifstream inFS;
    inFS.open("input.csv");
    //system("head input.csv");
    vector<double> nums;
    string line;
    while(getline(inFS, line))
    {
        nums.push_back(stod(line));
    }
    inFS.close();
    int N = nums.size();
    double ha[N];
    double *da;
    cudaMalloc((void **)&da, N*sizeof(double));
    for (int i = 0; i<N; ++i) {
        ha[i] = nums[i];
    }
    cudaMemcpy(da, ha, N*sizeof(double), cudaMemcpyHostToDevice);
    squareroot<<<N, 1>>>(da, N);
    ofstream outFS;
    outFS.open("output.csv");
    for(int i = 0; i<N; ++i)
    {
      outFS << hb[i] << endl;
    }
    outFS.close();
    cudaFree(da);
    cudaFree(db);
    return 0;
}