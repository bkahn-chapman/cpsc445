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
    vector<double> max_min;
    string line;
    while(getline(inFS, line))
    {
        int max_x = 0;
        int min_x = 0;
        int max_y = 0;
        int min_y = 0;
        int checkFirst = 0;
        for(char& c : line)
        {
            cout << c;
        }
    }
   
}