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
    cout << dna.length() << endl;
    int N = dna.length() / 3;
    string ha[N];
    string triplet;
    for(int i = 0; i < N; ++i)
    {
        triplet.push_back(dna[i]);
        triplet.push_back(dna[i+1]);
        triplet.push_back(dna[i+2]);
        ha[i] = triplet;
        triplet = "";
    }
    for(int i = 0; i < N; ++i)
    {
        cout << triplet[i] << endl;
    }
}