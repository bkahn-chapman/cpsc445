#include <iostream>
#include <vector>
#include <fstream>
#include <stdio.h>
#include <cmath>
#include <array>
using namespace std;

int main () {
    ifstream inFS;
    inFS.open("input.txt");
    string dna;
    getline(inFS, dna);
    inFS.close();
    ofstream outFS;
    outFS.open("output.txt")
    outFS << dna;
    outFS.close();
}