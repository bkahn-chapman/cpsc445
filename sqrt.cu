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
    vector<float> nums;
    string num;
    while(!inFS.eof())
    {
        getline(inFS, num);
        nums.push_back(stof(num));
    }
    cout << nums.size() << endl;
}