#include <string>
#include <fstream>
#include <iostream>
#include <vector>
using namespace std;

extern __shared__ vector<int> corners;

__global__
void find_corners(int N)
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
    find_corners<<<N, 1>>>(N);
}