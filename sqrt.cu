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
    string dna;
    getline(inFS, dna);
    inFS.close();
    ofstream outFS;
    outFS.open("output.csv");
    outFS << dna;
    outFS.close();
}