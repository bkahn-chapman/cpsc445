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

//checks if the cell being checked exists
int get(const std::vector<std::vector<int>> & board, int i, int j) {
    if (i<0 || i>=board.size()) {
        return 0;
    }
    if (j<0 || j>=board[i].size()) {
        return 0;
    }
    return board[i][j];
}

//determines what the new value as a specific cell should be
int new_value(const vector<vector<int>> & board, int i, int j) {
    int neighborCount = 0; //tracks how many neighbors the cell has
    neighborCount = neighborCount + get(board, i-1, j);
    neighborCount = neighborCount + get(board, i-1, j-1);
    neighborCount = neighborCount + get(board, i-1, j+1);
    neighborCount = neighborCount + get(board, i, j-1);
    neighborCount = neighborCount + get(board, i, j+1);
    neighborCount = neighborCount + get(board, i+1, j-1);
    neighborCount = neighborCount + get(board, i+1, j);
    neighborCount = neighborCount + get(board, i+1, j+1);
    //Game of Life logic
    if(board[i][j] == 1 && (neighborCount < 2 || neighborCount > 3))
    {
        return 0;
    }
    else if(board[i][j] == 0 && neighborCount == 3)
    {
        return 1;
    }
    else
    {
        return board[i][j];
    }
}

int main(int argc, char** argv)
{
    //checks to see if the proper number of arguments are provided
    if((argc-1) != 4)
    {
        cout << "invalid arguemnts (expected 4 arguments, instead received " << argc-1 << ")" << endl;
        exit(1);
    }
    string inputFile = argv[1]; //takes in the file to be read
    ifstream inFS; //opens the input filestream
    inFS.open(inputFile); //opens the given input file
    //if the file does not exist, reports it and closes the program
    if(!inFS)
    {
        cout << "invalid arguments (the input file was not found)" << endl;
        exit(1);
    }
    string outputFile = argv[2]; //takes in the file to be written to
    string textFile = ".txt"; //to check if the output file is valid
    //if the output file is not in a valid format (***.txt), reports it and closes the program
    if(outputFile.size() <= textFile.size() || outputFile.compare(outputFile.size() - textFile.size(), textFile.size(), textFile) != 0)
    {
        cout << "invalid arguments (the output file given is not in .txt format)" << endl;
        exit(1);
    }
    //takes in the number of steps and number of threads as strings to test if they are valid whole numbers
    string testThree = argv[3];
    string testFour = argv[4];
    //tests to see if the number of steps is a whole number, if not, reports and closes the program
    for(char const &c : testThree)
    {
        if(isdigit(c) == 0)
        {
            cout << "invalid arguments (number of steps must be a whole number)" << endl;
            exit(1);
        }
    }
    //tests to see if the number of threads is a whole number, if not, reports and closes the program
    for(char const &c : testFour)
    {
        if(isdigit(c) == 0)
        {
            cout << "invalid arguments (number of threads must be a whole number)" << endl;
            exit(1);
        }
    }
    //if they are valid, converts the number of steps and number of threads to integers
    int numSteps = atoi(argv[3]);
    int numThreads = atoi(argv[4]);
    //checks to ensure that at least 1 thread is specified, if not, reports and closes the program
    if(numThreads < 1)
    {
        cout << "invalid arguments (number of threads cannot be less than 1)" << endl;
        exit(1);
    }
    cout << "Welcome to Conway's Game of life!" << endl;
    string l; //line in file
    char c; //character in line
    int numRows = 0; //initializes the number of rows for the 2d vector
    int numColumns = 0; //initializes the number of columns for the 2d vector
    //gets the number of columns and rows for 2d vector initializing and also checks to ensure the given board is rectangular, if not, reports and exits the program
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
    inFS.clear(); //clears the input filestream
    inFS.seekg(0, std::ios::beg); //resets the input filestream to the beginning of the file
    //ensures that every character in the input file is valid, if not, reports and exits the program
    while(!inFS.eof())
    {
        inFS >> noskipws >> c;
        if(c != '0' && c != '1' && c != '\n')
        {
            cout << "invalid input (spaces and characters other than 0 and 1 are not allowed)" << endl;
            exit(1);
        }
    }
    inFS.clear(); //clears the input filestream
    inFS.seekg(0, std::ios::beg); //resets the input filestream to the beginning of the file
    vector<vector<int>> board; //initializes the game board
    vector<vector<int>> otherboard;
    //fills in the game board from the now confirmed-to-be-clean input file
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
        otherboard.push_back(row);
    }
    inFS.close(); //closes the input filestream
    ofstream outFS; //creates the output filestream
    outFS.open(outputFile); //opens/creates the given output file
    thread* myThreads = new thread[numThreads];
    //FIX: runs through each step given
    for(int s = 0; s < numSteps; ++s)
    {
        for(int i = 0; i < board.size(); ++i)
        {
            for(int j = 0; j < board[i].size(); ++j)
            {
                board[i][j] = new_value(otherboard, i, j);
            }
        }
        swap(board, otherboard);
    }
    delete[] myThreads;
    //outputs the results of the simulation into the given output file
    for(int i = 0; i < board.size(); ++i)
    {
        for(int j = 0; j < board[i].size(); ++j)
        {
            outFS << board[i][j];
        }
        outFS << "\n";
    }
    cout << "Thank you for using my program! Your results can be found at: " << outputFile << "." << endl;
}