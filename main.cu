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
        line.push_back(" ");
        string num;
        double max_x = 0;
        double min_x = 0;
        double max_y = 0;
        double min_y = 0;
        int checkFirst = 0;
        for(int i = 0; i < line.length(); ++i)
        {
            
        }
    }
   
}