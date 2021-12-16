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
void extreme(int *a, int *b, int c, int r, int N)
{

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
    int ha[N], hb[N];
    int *da, *db;
    cudaMalloc((void **)&da, N*sizeof(int));
    cudaMalloc((void **)&db, N*sizeof(int));
    for(int i = 0; i < N; ++i)
    {
        ha[i] = nums[i];
        hb[i] = 0;
    }
    cudaMemcpy(da, ha, N*sizeof(int), cudaMemcpyHostToDevice);
    extreme<<<N, 1>>>(da, db, colcount, rowcount, N);
    cudaMemcpy(hb, db, N*sizeof(int), cudaMemcpyHostToDevice);
    for(int i = 0; i < hb.size(); ++i)
    {
        cout << hb[i] << endl;
    }
}