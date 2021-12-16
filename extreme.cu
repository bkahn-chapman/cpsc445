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
    int rowcount = 0;
    int colcount = 1;
    string line;
    while(getline(inFS, line))
    {
        if(rowcount == 0)
        {
            for(char& c : line)
            {
                if(c == ',')
                {
                    colcount++;
                }
            }
        }
        rowcount++;
    }
    cout << colcount << endl;
    int nums[colcount][rowcount];
    int charcount = 0;
    while(inFS >> noskipws >> c)
    {
        if(c != ',' && c != '\n')
        {
            num.push_back(c);
        }
        else
        {
            nums[charcount/colcount][charcount%colcount] = num;
            num = "";
        }
        charcount++;
    }
}