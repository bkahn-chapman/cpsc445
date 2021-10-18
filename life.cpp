/*
* Name: Benjamin Kahn
* ID: 2316793
* Class: CPSC-445-01
* Assignment: Assignment 1
* About: Simulates the Game of Life using a given board, a given number of steps, and a given number of threads. Outputs to a given output file.
*/

#include <cmath>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <thread>
#include <unistd.h>
#include <vector>
using namespace std;

class NewGame
{
    public:
        vector<vector<int>> board; //initializes the game board
        vector<vector<int>> otherboard;
        int get(int i, int j);
        int new_value(int i, int j);
        void simRow(int currRow);
        string l; //line in file
        char c; //character in line
        int numRows = 0; //initializes the number of rows for the 2d vector
        int numColumns = 0; //initializes the number of columns for the 2d vector
        string inputFile;
        string outputFile;
        string inputSteps;
        string inputThreads;
        int numSteps = 0;
        int numThreads = 0;
        void checkArgs(int numArgs);
        void checkInput();
        void validOutput();
        void checkThreads();
        void checkSteps();
        void calcSize();
        void fileValid();
        void fillBoard();
        void maxThreads();
        void printResults();
        void inputValidity();
        void setup();
        void simAll();
};

//checks if the cell being checked exists
int NewGame::get(int i, int j) {
    if (i<0 || i>=otherboard.size()) {
        return 0;
    }
    if (j<0 || j>=otherboard[i].size()) {
        return 0;
    }
    return otherboard[i][j];
}

//determines what the new value as a specific cell should be
int NewGame::new_value(int i, int j) {
    int neighborCount = 0; //tracks how many neighbors the cell has
    neighborCount = neighborCount + get(i-1, j);
    neighborCount = neighborCount + get(i-1, j-1);
    neighborCount = neighborCount + get(i-1, j+1);
    neighborCount = neighborCount + get(i, j-1);
    neighborCount = neighborCount + get(i, j+1);
    neighborCount = neighborCount + get(i+1, j-1);
    neighborCount = neighborCount + get(i+1, j);
    neighborCount = neighborCount + get(i+1, j+1);
    //Game of Life logic
    if(otherboard[i][j] == 1 && (neighborCount < 2 || neighborCount > 3))
    {
        return 0;
    }
    else if(otherboard[i][j] == 0 && neighborCount == 3)
    {
        return 1;
    }
    else
    {
        return otherboard[i][j];
    }
}

//makes it so that each thread can simulate
void NewGame::simRow(int currRow)
{
    //simulates game of life
    for(int j = 0; j < board[j].size(); ++j)
    {
        board[currRow][j] = new_value(currRow, j);
    }
}

//checks to see if the proper number of arguments is given
void NewGame::checkArgs(int numArgs)
{
    if((numArgs-1) != 4) //makes sure there are exactly four arguments given
    {
        cout << "invalid arguemnts (expected 4 arguments, instead received " << numArgs-1 << ")" << endl;
        exit(1);
    }
}

//checks if the input file given exists
void NewGame::checkInput()
{
    ifstream inFS;
    inFS.open(inputFile);
    if(!inFS) //checks if the given file exists
    {
        cout << "One or more of the input files does not exist." << endl;
        exit(1);
    }
    inFS.close();
}

//checks if the given output file is valid
void NewGame::validOutput()
{
    string textFile = ".txt"; //to check if the output file is valid
    //if the output file is not in a valid format (***.txt), reports it and closes the program
    if(outputFile.size() <= textFile.size() || outputFile.compare(outputFile.size() - textFile.size(), textFile.size(), textFile) != 0)
    {
        cout << "invalid arguments (the output file given is not in .txt format)" << endl;
        exit(1);
    }
}

//checks if the number of threads given is valid
void NewGame::checkThreads()
{
    //makes sure every thread is a whole number
    for(char const &c : inputThreads)
    {
        if(isdigit(c) == 0)
        {
            cout << "invalid arguments (number of threads must be a whole number)" << endl;
            exit(1);
        }
    }
    //if they are valid, converts the number of steps and number of threads to integers
    numThreads = stoi(inputThreads);
    //checks to ensure that at least 1 thread is specified, if not, reports and closes the program
    if(numThreads < 1)
    {
        cout << "invalid arguments (number of threads cannot be less than 1)" << endl;
        exit(1);
    }
}

void NewGame::checkSteps()
{
    for(char const &c : inputSteps)
    {
        if(isdigit(c) == 0)
        {
            cout << "invalid arguments (number of steps must be a whole number)" << endl;
            exit(1);
        }
    }
    numSteps = stoi(inputSteps);
}

void NewGame::calcSize()
{
    ifstream inFS;
    inFS.open(inputFile);
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
    inFS.close();
}

void NewGame::fileValid()
{
    ifstream inFS;
    inFS.open(inputFile);
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
    inFS.close();
}

void NewGame::fillBoard()
{
    ifstream inFS;
    inFS.open(inputFile);
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
}

//ensures there aren't more threads than lines in the input file
void NewGame::maxThreads()
{
    //prevents there from being more threads than lines in the file as it would leave unused threads
    if(numThreads > numRows)
    {
        numThreads = numRows;
    }
}

void NewGame::printResults()
{
    ofstream outFS;
    outFS.open(outputFile, ios::app);
    //outputs the results of the simulation into the given output file
    for(int i = 0; i < otherboard.size(); ++i)
    {
        for(int j = 0; j < otherboard[i].size(); ++j)
        {
            outFS << otherboard[i][j];
        }
        outFS << "\n";
    }
    outFS.close();
}

void NewGame::inputValidity()
{
    checkInput();
    validOutput();
    checkSteps();
    checkThreads();
}

void NewGame::setup()
{
    calcSize();
    fileValid();
    fillBoard();
    maxThreads();
}

void NewGame::simAll()
{
    for(int i = 0; i < numSteps; ++i)
    {
        thread* myThreads = new thread[numThreads];
        for(int r = 0; r < numRows; ++r)
        {
            for(int t = 0; t < numThreads; ++t)
            {
                if(r % numThreads == t)
                {
                    myThreads[t] = thread(&NewGame::simRow, this, r);
                    myThreads[t].join();
                }
            }
        }
        delete[] myThreads;
        swap(board, otherboard);
    }
    printResults();
}

int main(int argc, char** argv)
{
    NewGame life;
    life.inputFile = argv[1]; //takes in the file to be read
    life.outputFile = argv[2]; //takes in the file to be written to
    life.inputSteps = argv[3];
    life.inputThreads = argv[4];
    cout << "Welcome to Conway's Game of life!" << endl;
    life.inputValidity();
    life.setup();
    life.simAll();
    cout << "Thank you for using my program! Your results can be found at: " << life.outputFile << "." << endl;
}