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
    //vector<int> nums;
    int rowcount = 0;
    int colcount = 1;
    string line;
    while(getline(inFS, line))
    {
        //cout << line << endl;
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
    cout << colcount << endl;
    cout << rowcount << endl;
    int nums[colcount][rowcount];
    for(int i = 0; i < colcount; ++i)
    {
        for(int j = 0; j < rowcount; ++j)
        {
            nums[i][j] = 0;
        }
    }
    int charcount = 0;
    char c;
    inFS.close();
    inFS.open("input.csv");
    while(inFS >> noskipws >> c)
    {
        if(c != ',' && c != '\n')
        {
            num.push_back(c);
        }
        else
        {
            nums[charcount/colcount][charcount%colcount] = stoi(num);
            num = "";
        }
        charcount++;
    }
    cout << endl;
    /*
    for(int i = 0; i < colcount; ++i)
    {
        for(int j = 0; j < rowcount; ++j)
        {
            cout << nums[i][j] << ",";
        }
        cout << endl;
    }
    */
}