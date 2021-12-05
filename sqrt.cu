#include <iostream>
#include <vector>
#include <fstream>
#include <stdio.h>
#include <cmath>
#include <array>
using namespace std;

int main () {
    ifstream inFS;
    inFS.open("input.csv");
    string test;
    while(!inFS.eof)
    {
        getline(inFS, test);
        cout << test << endl;
    }
}