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
void extreme(int *a, int *b, int N)
{
    int i = blockIdx.x;
    if (i<N) {
        b[i] = a[i]; 
        //no idea why a[i] isn't properly being accessed
        //plan was to have it check the eight surrounding and put a 1 in the associated slot in b
        //main would then determine which coordinate is at each value in b using the total number of rows and columns
        //cannot progress further though if this code in extreme isn't working as it did in sqrt and others
    }
}

int main () {
    ifstream inFS;
    inFS.open("input.csv");
    //system("head input.csv");
    string num;
    vector<int> nums;
    int rowcount = 0;
    int colcount = 1;
    string line;
    while(getline(inFS, line))
    {
        if(rowcount == 0)
        {
            for(char& d : line)
            {
                if(d == ',')
                {
                    colcount++;
                }
            }
        }
        rowcount++;
    }
    char c;
    ifstream if2;
    inFS.close();
    if2.open("input.csv");
    while(if2 >> noskipws >> c)
    {
        if(c == ',' || c == '\n')
        {
            nums.push_back(stoi(num));
            num = "";
        }
        else
        {
            num.push_back(c);
        }
    }
    int N = nums.size();
    cout << N << endl;
    int ha[N], hb[N];
    int *da, *db;
    cudaMalloc((void **)&da, N*sizeof(int));
    cudaMalloc((void **)&db, N*sizeof(int));
    for(int i = 0; i < N; ++i)
    {
        ha[i] = nums[i];
    }
    /*
    for(int i = 0; i < N; ++i)
    {
        cout << ha[i] << endl;
    }
    */
    cudaMemcpy(da, ha, N*sizeof(int), cudaMemcpyHostToDevice);
    extreme<<<N, 1>>>(da, db, N);
    cudaMemcpy(hb, db, N*sizeof(int), cudaMemcpyHostToDevice);
    for(int i = 0; i < N; ++i)
    {
        cout << hb[i] << endl;
    }
}