#include <iostream>
#include <vector>
#include <fstream>
#include <stdio.h>
#include <cmath>
#include <array>
#include <string>
#include <algorithm>
using namespace std;

int main () {
    ifstream inFS;
    inFS.open("dna.txt");
    string dna;
    getline(inFS, dna);
    int N = dna.length() / 3;
    string ha[N];
    for(int i = 0; i < ha.size(); ++i)
    {
        cout << ha[i] << endl;
    }
}