#include <string>
#include <fstream>
#include <iostream>
#include <vector>
using namespace std;

__global__
void find_corners(string *a, int N)
{
    corners.push_back(N);
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
    vecor<string> *da;
    cudaMalloc((void **)&da, N*sizeof(string));
    for(int i = 0; i<N; ++i)
    {
        ha[i] = polygons[i];
    }
    cudaMemcpy(ha, N*sizeof(string), cudaMemcpyHostToDevice);
    find_corners<<<N, 1>>>(ha, N);
    cudaMemcpy(hb, N, cudaMemcpyDeviceToHost);
}