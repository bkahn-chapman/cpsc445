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
        vector<vector<int>> otherboard; //initializes the second game board
        int get(int i, int j); //checks if the cell being checked exists
        int new_value(int i, int j); //determines what the new value as a specific cell should be
        void simRow(int currRow); //makes it so that each thread can simulate a specific row
        string l; //line in file
        char c; //character in line
        int numRows = 0; //initializes the number of rows for the 2d vector
        int numColumns = 0; //initializes the number of columns for the 2d vector
        string inputFile; //input file to be read
        string outputFile; //output file to be read
        string inputSteps; //string input of number of steps for checking
        string inputThreads; //string input of number of threads for checking
        int numSteps = 0; //final number of steps
        int numThreads = 0; //final number of threads
        void checkArgs(int numArgs); //checks to see if the proper number of arguments is given
        void checkInput(); //checks if the input file given exists
        void validOutput(); //checks if the given output file is valid
        void checkThreads(); //checks if the number of threads given is valid
        void checkSteps(); //checks if the number of steps given is valid
        void calcSize(); //calculates the dimensions of the given input board
        void fileValid(); //checks if the given input file has exclusively valid characters
        void fillBoard(); //given a valid board, fills the vector of ints that makes up the game board to be updated
        void maxThreads(); //ensures there aren't more threads than lines in the input file
        void printResults(); //prints the final board once all steps have completed
        void inputValidity(); //combines all validity checks into one statement
        void setup(); //combines all setup code into one statement
        void simAll(); //runs the Game of Life given a specific number of steps and outputs results
};

//checks if the cell being checked exists
int NewGame::get(int i, int j) {
    //if the row to be checked doesn't exist
    if (i<0 || i>=otherboard.size()) {
        return 0;
    }
    //if the column to be checked doesn't exist
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

//makes it so that each thread can simulate a specific row
void NewGame::simRow(int currRow)
{
    //simulates game of life
    for(int j = 0; j < board[currRow].size(); ++j)
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

//checks if the number of steps given is valid
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

//calculates the dimensions of the given input board
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

//checks if the given input file has exclusively valid characters
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

//given a valid board, fills the vector of ints that makes up the game board to be updated
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

//prints the final board once all steps have completed
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

//combines all validity checks into one statement
void NewGame::inputValidity()
{
    checkInput();
    validOutput();
    checkSteps();
    checkThreads();
}

//combines all setup code into one statement
void NewGame::setup()
{
    calcSize();
    fileValid();
    fillBoard();
    maxThreads();
}

//runs the Game of Life given a specific number of steps and outputs results
void NewGame::simAll()
{
    for(int i = 0; i < numSteps; ++i)
    {
        //creates a new set of threads for each step
        thread* myThreads = new thread[numThreads];
        for(int r = 0; r < numRows; ++r)
        {
            for(int t = 0; t < numThreads; ++t)
            {
                //ensures that each thread gets its own row
                if(r % numThreads == t)
                {
                    myThreads[t] = thread(&NewGame::simRow, this, r);
                    myThreads[t].join();
                }
            }
        }
        delete[] myThreads;
        swap(board, otherboard); //so that checks can be performed on the new board
    }
    printResults();
}

int main(int argc, char** argv)
{
    NewGame life;
    life.inputFile = argv[1]; //takes in the file to be read
    life.outputFile = argv[2]; //takes in the file to be written to
    life.inputSteps = argv[3]; //takes in the desired number of steps
    life.inputThreads = argv[4]; //takes in the desired number of threads
    cout << "Welcome to Conway's Game of life!" << endl;
    life.inputValidity(); //checks if all input is valid
    life.setup(); //sets up the game board
    life.simAll(); //simulates all steps
    cout << "Thank you for using my program! Your results can be found at: " << life.outputFile << "." << endl;
}