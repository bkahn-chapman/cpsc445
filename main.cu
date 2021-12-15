#include <string>
#include <fstream>
#include <iostream>
#include <vector>
using namespace std;

__global__
void find_corners(string *a, int *b, int N)
{
    
}

int main () {
    ifstream inFS;
    inFS.open("input.txt");
    vector<string> polygons;
    string line;
    while(getline(inFS, line))
    {
        polygons.push_back(line);
    }
    int N = polygons.size();
    vector<string> ha;
    double hb[N*4];
    vector<string> *da;
    double *db;
    cudaMalloc((void **)&da, N*sizeof(string));
    cudaMalloc((void **)&db, N*sizeof(int));
    for(int i = 0; i<N; ++i)
    {
        ha[i] = polygons[i];
    }
    cout << ha[i] << endl;
    cudaMemcpy(da, ha, N*sizeof(string), cudaMemcpyHostToDevice);
    find_corners<<<N, 1>>>(da, db, N);
    cudaMemcpy(hb, db, N*sizeof(int), cudaMemcpyDeviceToHost);
}