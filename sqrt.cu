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
    string line;
    while(getline(inFS, line))
    {
        string num = "";
        for(char &c : line)
        {
            if(c == '\n')
            {
                cout << num << endl;
            }  
            else
            {
                num.push_back(c);
            }
        }
    }
}