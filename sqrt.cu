#include <iostream>
#include <vector>
#include <fstream>
#include <stdio.h>
#include <cmath>
#include <array>
#include <string>
using namespace std;

int main () {
    ifstream inFS;
    inFS.open("input.csv");
    system("head input.csv");
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
}