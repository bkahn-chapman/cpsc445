#include <iostream>

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