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
    vector<int> nums;
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
    int charcount = 0;
    char c;
    ifstream if2;
    inFS.close();
    if2.open("input.csv");
    while(if2 >> noskipws >> c)
    {
        if(c == ',' || c == '\n')
        {
            nums.push_back(stoi(c));
            num = "";
        }
        else
        {
            num.push_back(c);
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