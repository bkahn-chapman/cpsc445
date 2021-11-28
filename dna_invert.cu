#include <iostream>
#include <vector>
#include <fstream>
#include <stdio.h>
#include <cmath>
#include <array>
using namespace std;

__global__

int main () {
    ifstream inFS;
    inFS.open("dna.txt")
    string dna;
    getline(inFS, dna);
    cout << dna << endl;
}