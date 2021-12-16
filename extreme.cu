#include <iostream>
#include <vector>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <cmath>
#include <array>
#include <string>
using namespace std;

int main () {
    ifstream inFS;
    inFS.open("input.csv");
    //system("head input.csv");
    string num;
    char c;
    int rowcount;
    int colcount;
    string line;
    while(getline(inFS, line))
    {
        if(rowcount == 0)
        {
            for(char& c : line)
            {
                if(c != ',' && c != '\n')
                {
                    colcount++;
                }
            }
        }
        rowcount++;
    }
    cout << colcount << endl;
    while(inFS >> noskipws >> c)
    {
        if(c != ',' && c != '\n')
        {
            num.push_back(c);
        }
        else
        {
            nums.push_back(stoi(num));
            num = "";
        }
    }

}