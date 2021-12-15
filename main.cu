#include <string>
#include <fstream>
#include <iostream>
#include <vector>

int main () {
    ifstream inFS;
    inFS.open("input.csv");
    vector<string> polygons;
    string line;
    while(getline(inFS, line))
    {
        polygons.push_back(line);
    }
    for(int i = 0; i < polygons.size(); ++i)
    {
        cout << polygons[i] << endl;
    }
}