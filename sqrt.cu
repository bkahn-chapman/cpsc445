#include <iostream>
#include <fstream>
#include <string>

int main () {
    ifstream inFS;
    inFS.open("input.txt");
    string dna;
    getline(inFS, dna);
    inFS.close();
    ofstream outFS;
    outFS << dna;
    outFS.close();
}