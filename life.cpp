#include <unistd.h>
#include <iostream>
#include <fstream>
#include <cmath>
#include <string>
#include <cstdlib>
#include <thread>
#include <mutex>
#include <typeinfo>
#include <vector>
using namespace std;

int get(const std::vector<std::vector<int>> & board, int i, int j) {
    if (i<0 || i>=board.size()) {
        return 0;
    }
    if (j<0 || j>=board[i].size()) {
        return 0;
    }
    return board[i][j];
}

int new_value(const vector<vector<int>> & board, int i, int j) {
    int neighborCount = 0;
    neighborCount = neighborCount + get(board, i-1, j);
    neighborCount = neighborCount + get(board, i-1, j-1);
    neighborCount = neighborCount + get(board, i-1, j+1);
    neighborCount = neighborCount + get(board, i, j-1);
    neighborCount = neighborCount + get(board, i, j+1);
    neighborCount = neighborCount + get(board, i+1, j-1);
    neighborCount = neighborCount + get(board, i+1, j);
    neighborCount = neighborCount + get(board, i+1, j+1);
    if(neighborCount < 2)
    {
        return 0;
    }
    else if(neighborCount < 4)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

int main(int argc, char** argv)
{
    if((argc-1) != 4)
    {
        cout << "invalid arguemnts (expected 4 arguments, instead received " << argc-1 << ")" << endl;
        exit(1);
    }
    string inputFile = argv[1];
    ifstream inFS;
    inFS.open(inputFile);
    if(!inFS)
    {
        cout << "invalid arguments (the input file was not found)" << endl;
        exit(1);
    }
    string outputFile = argv[2];
    string textFile = ".txt";
    if(outputFile.size() <= textFile.size() || outputFile.compare(outputFile.size() - textFile.size(), textFile.size(), textFile) != 0)
    {
        cout << "invalid arguments (the output file given is not in .txt format)" << endl;
        exit(1);
    }
    string testThree = argv[3];
    string testFour = argv[4];
    for(char const &c : testThree)
    {
        if(isdigit(c) == 0)
        {
            cout << "invalid arguments (number of steps must be a whole number)" << endl;
            exit(1);
        }
    }
    for(char const &c : testFour)
    {
        if(isdigit(c) == 0)
        {
            cout << "invalid arguments (number of threads must be a whole number)" << endl;
            exit(1);
        }
    }
    int numSteps = atoi(argv[3]);
    int numThreads = atoi(argv[4]);
    if(numThreads < 1)
    {
        cout << "invalid arguments (number of threads cannot be less than 1)" << endl;
        exit(1);
    }
    cout << "Welcome to Conway's Game of life!" << endl;
    string l;
    char c;
    int numRows = 0;
    int numColumns = 0;
    while(getline(inFS, l))
    {
        if(numColumns == 0)
        {
            numColumns = l.length()-1;
        }
        if(l.length()-1 != numColumns)
        {
            cout << "invalid input (the given board is not rectangular)" << endl;
            exit(1);
        }
        numRows++;
    }
    numColumns++;
    inFS.clear();
    inFS.seekg(0, std::ios::beg);
    while(!inFS.eof())
    {
        inFS >> noskipws >> c;
        if(c != '0' && c != '1' && c != '\n')
        {
            cout << "invalid input (spaces and characters other than 0 and 1 are not allowed)" << endl;
            exit(1);
        }

    }
    inFS.clear();
    inFS.seekg(0, std::ios::beg);
    vector<vector<int>> board;
    vector<vector<int>> newboard;
    while(getline(inFS, l))
    {
        vector<int> row;
        for(char &c : l)
        {
            if(c!='\n')
            {
                if(c == '0')
                {
                    row.push_back(0);
                }
                else if(c == '1')
                {
                    row.push_back(1);
                }
            }
        }
        board.push_back(row);
    }
    inFS.close();  
    ofstream outFS;
    outFS.open(outputFile);
    for(int s = 0; s < numSteps; ++s)
    {
        for(int i = 0; i < board.size(); ++i)
        {
            for(int j = 0; j < board[i].size(); ++j)
            {
                outFS << new_value(board, i, j);
            }
            outFS << '\n';
        }
    }
    if(numSteps == 0)
    {
        for(int i = 0; i < board.size(); ++i)
        {
            for(int j = 0; j < board[i].size(); ++j)
            {
                outFS << board[i][j];
            }
            outFS << "\n";
        }
    }
    cout << "Thank you for using my program! Your results can be found at: " << outputFile << "." << endl;
}