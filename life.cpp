#include <unistd.h>
#include <iostream>
#include <fstream>
#include <cmath>
#include <string>
#include <cstdlib>
#include <thread>
#include <mutex>
#include <typeinfo>
using namespace std;

class life
{
  public:
    life(); //default constructor
    ~life(); //destructor

    //check each possible cell position
    int CheckCornerTL(int row, int column, int currentGen);
    int CheckCornerTR(int row, int column, int currentGen);
    int CheckCornerBL(int row, int column, int currentGen);
    int CheckCornerBR(int row, int column, int currentGen);
    int CheckSideT(int row, int column, int currentGen);
    int CheckSideR(int row, int column, int currentGen);
    int CheckSideB(int row, int column, int currentGen);
    int CheckSideL(int row, int column, int currentGen);
    int CheckMiddle(int row, int column, int currentGen);

    int numRows; //number of rows in the map
    int numColumns; //number of columns in the map
    int genNum; //tracks the current step number
    int numSteps;
    int numThreads;

    void CreateMap(int rows, int columns, string fileName);
    void NewGen(int currentGen); //generates the next map alternating
    void PrintResult(string outputFile);
    
    int** map1; //first map
    int** map2; //second map
};

//class constructor
life::life()
{
    cout << "New Game of life instance created." << endl;
}

//class destructor
life::~life()
{
    cout << "Game of life instance ended successfully." << endl;
}

void life::CreateMap(int rows, int columns, string fileName)
{
  //creates the map for the first map
  map1 = new int*[rows];
  for(int i = 0; i < rows; ++i)
  {
    map1[i] = new int[columns];
    for(int j = 0; j < columns; ++j)
    {
      map1[i][j] = 0;
    }
  }

  //creates the map for the second map
  map2 = new int*[rows];
  for(int i = 0; i < rows; ++i)
  {
    map2[i] = new int[columns];
    for(int j = 0; j < columns; ++j)
    {
      map2[i][j] = 0;
    }
  }

  //opens the file to be read
  ifstream inFS;
  inFS.open(fileName);
  char c; //the current character being read
  int rowCount = 0; //the number of rows in the input
  int columnCount = 0; //the number of columns in the input
  //while the document is not empty
  while(!inFS.eof())
  {
    inFS >> c;
    for(int i = 0; i < rows; ++i)
    {
        columnCount = 0;
        for(int j = 0; j < columns; ++j)
        {
            map2[rowCount][columnCount] = c;
            columnCount++;
        }
        rowCount++;
    }
    rowCount = 0;
  }
  inFS.close(); //closes the file
}

//counts and returns all of the possible neighbors for the cell in the top left corner (takes in the position of the cell)
int life::CheckCornerTL(int row, int column, int currentGen)
{
    int numAround = 0;
    if(currentGen == 0)
    {

    }
    else if(currentGen % 2 == 0)
    {
        if(map1[row][column+1] == '1')
        {
            numAround++;
        }
        if(map1[row+1][column+1] == '1')
        {
            numAround++;
        }
        if(map1[row+1][column] == '1')
        {
            numAround++;
        }
    }
    else
    {
        if(map2[row][column+1] == '1')
        {
            numAround++;
        }
        if(map2[row+1][column+1] == '1')
        {
            numAround++;
        }
        if(map2[row+1][column] == '1')
        {
            numAround++;
        }
    }
    return numAround;
}

//counts and returns all of the possible neighbors for the cell in the top right corner (takes in the position of the cell)
int life::CheckCornerTR(int row, int column, int currentGen)
{
    int numAround = 0;
    if(currentGen == 0)
    {

    }
    else if(currentGen % 2 == 0)
    {
        if(map1[row+1][column] == '1')
        {
            numAround++;
        }
        if(map1[row+1][column-1] == '1')
        {
            numAround++;
        }
        if(map1[row][column-1] == '1')
        {
            numAround++;
        }
    }
    else
    {
        if(map2[row+1][column] == '1')
        {
            numAround++;
        }
        if(map2[row+1][column-1] == '1')
        {
            numAround++;
        }
        if(map2[row][column-1] == '1')
        {
            numAround++;
        }
    }
    return numAround;
}

//counts and returns all of the possible neighbors for the cell in the bottom left corner (takes in the position of the cell)
int life::CheckCornerBL(int row, int column, int currentGen)
{
    int numAround = 0;
    if(currentGen == 0)
    {

    }
    else if(currentGen % 2 == 0)
    {
        if(map1[row][column+1] == '1')
        {
            numAround++;
        }
        if(map1[row-1][column+1] == '1')
        {
            numAround++;
        }
        if(map1[row-1][column] == '1')
        {
            numAround++;
        }
    }
    else
    {
        if(map2[row][column+1] == '1')
        {
            numAround++;
        }
        if(map2[row-1][column+1] == '1')
        {
            numAround++;
        }
        if(map2[row-1][column] == '1')
        {
            numAround++;
        }
    }
    return numAround;
}

//counts and returns all of the possible neighbors for the cell in the bottom right corner (takes in the position of the cell)
int life::CheckCornerBR(int row, int column, int currentGen)
{
    int numAround = 0;
    if(currentGen == 0)
    {

    }
    else if(currentGen % 2 == 0)
    {
        if(map1[row-1][column] == '1')
        {
            numAround++;
        }
        if(map1[row-1][column-1] == '1')
        {
            numAround++;
        }
        if(map1[row][column-1] == '1')
        {
            numAround++;
        }
    }
    else
    {
        if(map2[row-1][column] == '1')
        {
            numAround++;
        }
        if(map2[row-1][column-1] == '1')
        {
            numAround++;
        }
        if(map2[row][column-1] == '1')
        {
            numAround++;
        }
    }
    return numAround;
}

//counts and returns all of the possible neighbors for the cells on the top side (takes in the position of the cell)
int life::CheckSideT(int row, int column, int currentGen)
{
    int numAround = 0;
    if(currentGen == 0)
    {

    }
    else if(currentGen % 2 == 0)
    {
        if(map1[row][column-1] == '1')
        {
            numAround++;
        }
        if(map1[row][column+1] == '1')
        {
            numAround++;
        }
        if(map1[row+1][column-1] == '1')
        {
            numAround++;
        }
        if(map1[row+1][column] == '1')
        {
            numAround++;
        }
        if(map1[row+1][column+1] == '1')
        {
            numAround++;
        }
    }
    else
    {
        if(map2[row][column-1] == '1')
        {
            numAround++;
        }
        if(map2[row][column+1] == '1')
        {
            numAround++;
        }
        if(map2[row+1][column-1] == '1')
        {
            numAround++;
        }
        if(map2[row+1][column] == '1')
        {
            numAround++;
        }
        if(map2[row+1][column+1] == '1')
        {
            numAround++;
        }
    }
    return numAround;
}

//counts and returns all of the possible neighbors for the cells on the right side (takes in the position of the cell)
int life::CheckSideR(int row, int column, int currentGen)
{
    int numAround = 0;
    if(currentGen == 0)
    {

    }
    else if(currentGen % 2 == 0)
    {
        if(map1[row-1][column-1] == '1')
        {
            numAround++;
        }
        if(map1[row-1][column] == '1')
        {
            numAround++;
        }
        if(map1[row][column-1] == '1')
        {
            numAround++;
        }
        if(map1[row+1][column-1] == '1')
        {
            numAround++;
        }
        if(map1[row+1][column] == '1')
        {
            numAround++;
        }
    }
    else
    {
        if(map2[row-1][column-1] == '1')
        {
            numAround++;
        }
        if(map2[row-1][column] == '1')
        {
            numAround++;
        }
        if(map2[row][column-1] == '1')
        {
            numAround++;
        }
        if(map2[row+1][column-1] == '1')
        {
            numAround++;
        }
        if(map2[row+1][column] == '1')
        {
            numAround++;
        }
    }
    return numAround;
}

//counts and returns all of the possible neighbors for the cells on the bottom side (takes in the position of the cell)
int life::CheckSideB(int row, int column, int currentGen)
{
    int numAround = 0;
    if(currentGen == 0)
    {

    }
    else if(currentGen % 2 == 0)
    {
        if(map1[row][column-1] == '1')
        {
            numAround++;
        }
        if(map1[row][column+1] == '1')
        {
            numAround++;
        }
        if(map1[row-1][column-1] == '1')
        {
            numAround++;
        }
        if(map1[row-1][column] == '1')
        {
            numAround++;
        }
        if(map1[row-1][column+1] == '1')
        {
            numAround++;
        }
    }
    else
    {
        if(map2[row][column-1] == '1')
        {
            numAround++;
        }
        if(map2[row][column+1] == '1')
        {
            numAround++;
        }
        if(map2[row-1][column-1] == '1')
        {
            numAround++;
        }
        if(map2[row-1][column] == '1')
        {
            numAround++;
        }
        if(map2[row-1][column+1] == '1')
        {
            numAround++;
        }
    }
    return numAround;
}

//counts and returns all of the possible neighbors for the cells on the left side (takes in the position of the cell)
int life::CheckSideL(int row, int column, int currentGen)
{
    int numAround = 0;
    if(currentGen == 0)
    {

    }
    else if(currentGen % 2 == 0)
    {
        if(map1[row-1][column] == '1')
        {
            numAround++;
        }
        if(map1[row-1][column+1] == '1')
        {
            numAround++;
        }
        if(map1[row][column+1] == '1')
        {
            numAround++;
        }
        if(map1[row+1][column+1] == '1')
        {
            numAround++;
        }
        if(map1[row+1][column] == '1')
        {
            numAround++;
        }
    }
    else
    {
        if(map2[row-1][column] == '1')
        {
            numAround++;
        }
        if(map2[row-1][column+1] == '1')
        {
            numAround++;
        }
        if(map2[row][column+1] == '1')
        {
            numAround++;
        }
        if(map2[row+1][column+1] == '1')
        {
            numAround++;
        }
        if(map2[row+1][column] == '1')
        {
            numAround++;
        }
    }
    return numAround;
}

//counts and returns all of the possible neighbors for the cells in the middle (takes in the position of the cell)
int life::CheckMiddle(int row, int column, int currentGen)
{
    int numAround = 0;
    if(currentGen == 0)
    {

    }
    else if(currentGen % 2 == 0)
    {
        if(map1[row][column-1] == '1')
        {
            numAround++;
        }
        if(map1[row][column+1] == '1')
        {
            numAround++;
        }
        if(map1[row-1][column-1] == '1')
        {
            numAround++;
        }
        if(map1[row-1][column] == '1')
        {
            numAround++;
        }
        if(map1[row-1][column+1] == '1')
        {
            numAround++;
        }
        if(map1[row+1][column-1] == '1')
        {
            numAround++;
        }
        if(map1[row+1][column] == '1')
        {
            numAround++;
        }
        if(map1[row+1][column+1] == '1')
        {
            numAround++;
        }
    }
    else
    {
        if(map2[row][column-1] == '1')
        {
            numAround++;
        }
        if(map2[row][column+1] == '1')
        {
            numAround++;
        }
        if(map2[row-1][column-1] == '1')
        {
            numAround++;
        }
        if(map2[row-1][column] == '1')
        {
            numAround++;
        }
        if(map2[row-1][column+1] == '1')
        {
            numAround++;
        }
        if(map2[row+1][column-1] == '1')
        {
            numAround++;
        }
        if(map2[row+1][column] == '1')
        {
            numAround++;
        }
        if(map2[row+1][column+1] == '1')
        {
            numAround++;
        }
    }
    return numAround;
}

//creates the next step
void life::NewGen(int currentGen)
{
    if(currentGen % 2 == 0)
    {
        int numAround; //tracks the number of neighbors for the cell

        for(int i = 0; i < numRows; ++i)
        {
            for(int j = 0; j < numColumns; ++j){
                numAround = 0; //resets the number of neighbors
                //if in row 1
                if(i == 0){
                    //if in row 1, column 1
                    if(j == 0){
                        numAround = CheckCornerTL(i, j, currentGen);
                    }
                    //if in row 1, last column
                    else if(j == (numColumns - 1)){
                        numAround = CheckCornerTR(i, j, currentGen);
                    }
                    //anywhere else in row 1
                    else
                    {       
                        numAround = CheckSideT(i, j, currentGen);
                    }
                }
                //if in last row
                else if(i == (numRows - 1))
                {
                    //if in last row, column 1
                    if(j == 0)
                    {
                        numAround = CheckCornerBL(i, j, currentGen);
                    }
                    //if in last row, last column
                    else if(j == (numColumns - 1))
                    {
                        numAround = CheckCornerBR(i, j, currentGen);
                    }
                    //anywhere else in the last row
                    else
                    {
                        numAround = CheckSideB(i, j, currentGen);
                    }
                }
                //if column 0
                else if(j == 0)
                {
                    //if any other row, column 0
                    if(i != 0 && i != (numRows -1))
                    {
                        numAround = CheckSideL(i, j, currentGen);
                    }
                }
                //if last column
                else if(j == (numColumns - 1))
                {
                    //if any other row, last column
                    if(i != 0 && i != (numRows -1))
                    {
                        numAround = CheckSideR(i, j, currentGen);
                    }
                }
                //if it is not on the border anywhere
                else
                {
                    numAround = CheckMiddle(i, j, currentGen);
                }

                //if the cell has one or zero neighbors it dies
                if(numAround <= 1)
                {
                    map2[i][j] = '0';
                }
                //if the cell has two neighbors it remains the same
                else if(numAround < 3)
                {         

                }
                //if the cell has three neighbors it gets filled
                else if(numAround < 4)
                {
                    map2[i][j] = '1';
                }
                //if the cell has four, five, six, seven, or eight neighbors it dies
                else
                {
                    map2[i][j] = '0';
                } 
            }       
        }
    }
    else
    {
        int numAround; //tracks the number of neighbors for the cell

        for(int i = 0; i < numRows; ++i)
        {
            for(int j = 0; j < numColumns; ++j){
                numAround = 0; //resets the number of neighbors
                //if in row 1
                if(i == 0){
                    //if in row 1, column 1
                    if(j == 0){
                        numAround = CheckCornerTL(i, j, currentGen);
                    }
                    //if in row 1, last column
                    else if(j == (numColumns - 1)){
                        numAround = CheckCornerTR(i, j, currentGen);
                    }
                    //anywhere else in row 1
                    else
                    {       
                        numAround = CheckSideT(i, j, currentGen);
                    }
                }
                //if in last row
                else if(i == (numRows - 1))
                {
                    //if in last row, column 1
                    if(j == 0)
                    {
                        numAround = CheckCornerBL(i, j, currentGen);
                    }
                    //if in last row, last column
                    else if(j == (numColumns - 1))
                    {
                        numAround = CheckCornerBR(i, j, currentGen);
                    }
                    //anywhere else in the last row
                    else
                    {
                        numAround = CheckSideB(i, j, currentGen);
                    }
                }
                //if column 0
                else if(j == 0)
                {
                    //if any other row, column 0
                    if(i != 0 && i != (numRows -1))
                    {
                        numAround = CheckSideL(i, j, currentGen);
                    }
                }
                //if last column
                else if(j == (numColumns - 1))
                {
                    //if any other row, last column
                    if(i != 0 && i != (numRows -1))
                    {
                        numAround = CheckSideR(i, j, currentGen);
                    }
                }
                //if it is not on the border anywhere
                else
                {
                    numAround = CheckMiddle(i, j, currentGen);
                }

                //if the cell has one or zero neighbors it dies
                if(numAround <= 1)
                {
                    map1[i][j] = '0';
                }
                //if the cell has two neighbors it remains the same
                else if(numAround < 3)
                {         

                }
                //if the cell has three neighbors it gets filled
                else if(numAround < 4)
                {
                    map1[i][j] = '1';
                }
                //if the cell has four, five, six, seven, or eight neighbors it dies
                else
                {
                    map1[i][j] = '0';
                } 
            }       
        }
    } 
}

void life::PrintResult(string outputFile)
{
    //opens the file output to the file name of the user's choice
    ofstream outFS;
    outFS.open(outputFile, ios::app);
    //prints the map
    for(int i = 0; i < numRows; ++i)
    {
        for(int j = 0; j < numColumns; ++j)
        {
            outFS << map2[i][j];
        }
        outFS << endl;
    }
    outFS.close(); //closes the file output
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
    inFS.close();
    ifstream inFD;
    inFD.open(inputFile);
    while(!inFD.eof())
    {
        inFD >> noskipws >> c;
        if(c != '0' && c != '1' && c != '\n')
        {
            cout << "invalid input (spaces and characters other than 0 and 1 are not allowed)" << endl;
            exit(1);
        }
    }
    inFD.close();

    life *newLife = new life();
    newLife -> CreateMap(numRows, numColumns, inputFile);
    for(int i = 0; i <= numSteps; ++i)
    {
        newLife -> NewGen(i);
    }
    newLife -> PrintResult(outputFile);
    delete newLife;
    cout << "Thank you for using my program! Your results can be found at: " << outputFile << "." << endl;
}