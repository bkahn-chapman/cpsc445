#include <unistd.h>
#include <iostream>
#include <fstream>
#include <cmath>
#include <string>
#include <cstdlib>
#include <thread>
#include <mutex>
using namespace std;

class life
{
  public:
    life(); //default constructor
    ~life(); //destructor

    //check each possible cell position
    int CheckCornerTL(int row, int column);
    int CheckCornerTR(int row, int column);
    int CheckCornerBL(int row, int column);
    int CheckCornerBR(int row, int column);
    int CheckSideT(int row, int column);
    int CheckSideR(int row, int column);
    int CheckSideB(int row, int column);
    int CheckSideL(int row, int column);
    int CheckMiddle(int row, int column);

    int numRows; //number of rows in the map
    int numColumns; //number of columns in the map
    int genNum; //tracks the current generation number

    void NewGen(); //generates the next map and sends the old one to the previous map
    void PrintMap(string outputFile); //prints the current map
   
    char** previousMap; //old map
    char** currentMap; //new map
};

//class constructor
life::life()
{
    cout << "New Game of Life instance created." << endl;
}

//class destructor
life::~life()
{
    cout << "Game of Life instance ended successfully." << endl;
}

//counts and returns all of the possible neighbors for the cell in the top left corner (takes in the position of the cell)
int life::CheckCornerTL(int row, int column)
{
  int numAround = 0;
  if(previousMap[row][column+1] == '1')
  {
    numAround++;
  }
  if(previousMap[row+1][column+1] == '1')
  {
    numAround++;
  }
  if(previousMap[row+1][column] == '1')
  {
    numAround++;
  }
  return numAround;
}

//counts and returns all of the possible neighbors for the cell in the top right corner (takes in the position of the cell)
int life::CheckCornerTR(int row, int column)
{
  int numAround = 0;
  if(previousMap[row+1][column] == '1')
  {
    numAround++;
  }
  if(previousMap[row+1][column-1] == '1')
  {
    numAround++;
  }
  if(previousMap[row][column-1] == '1')
  {
    numAround++;
  }
  return numAround;
}

//counts and returns all of the possible neighbors for the cell in the bottom left corner (takes in the position of the cell)
int life::CheckCornerBL(int row, int column)
{
  int numAround = 0;
  if(previousMap[row][column+1] == '1')
  {
    numAround++;
  }
  if(previousMap[row-1][column+1] == '1')
  {
    numAround++;
  }
  if(previousMap[row-1][column] == '1')
  {
    numAround++;
  }
  return numAround;
}

//counts and returns all of the possible neighbors for the cell in the bottom right corner (takes in the position of the cell)
int life::CheckCornerBR(int row, int column)
{
  int numAround = 0;
  if(previousMap[row-1][column] == '1')
  {
    numAround++;
  }
  if(previousMap[row-1][column-1] == '1')
  {
    numAround++;
  }
  if(previousMap[row][column-1] == '1')
  {
    numAround++;
  }
  return numAround;
}

//counts and returns all of the possible neighbors for the cells on the top side (takes in the position of the cell)
int life::CheckSideT(int row, int column)
{
  int numAround = 0;
  if(previousMap[row][column-1] == '1')
  {
    numAround++;
  }
  if(previousMap[row][column+1] == '1')
  {
    numAround++;
  }
  if(previousMap[row+1][column-1] == '1')
  {
    numAround++;
  }
  if(previousMap[row+1][column] == '1')
  {
    numAround++;
  }
  if(previousMap[row+1][column+1] == '1')
  {
    numAround++;
  }
  return numAround;
}

//counts and returns all of the possible neighbors for the cells on the right side (takes in the position of the cell)
int life::CheckSideR(int row, int column)
{
  int numAround = 0;
  if(previousMap[row-1][column-1] == '1')
  {
    numAround++;
  }
  if(previousMap[row-1][column] == '1')
  {
    numAround++;
  }
  if(previousMap[row][column-1] == '1')
  {
    numAround++;
  }
  if(previousMap[row+1][column-1] == '1')
  {
    numAround++;
  }
  if(previousMap[row+1][column] == '1')
  {
    numAround++;
  }
  return numAround;
}

//counts and returns all of the possible neighbors for the cells on the bottom side (takes in the position of the cell)
int life::CheckSideB(int row, int column)
{
  int numAround = 0;
  if(previousMap[row][column-1] == '1')
  {
    numAround++;
  }
  if(previousMap[row][column+1] == '1')
  {
    numAround++;
  }
  if(previousMap[row-1][column-1] == '1')
  {
    numAround++;
  }
  if(previousMap[row-1][column] == '1')
  {
    numAround++;
  }
  if(previousMap[row-1][column+1] == '1')
  {
    numAround++;
  }
  return numAround;
}

//counts and returns all of the possible neighbors for the cells on the left side (takes in the position of the cell)
int life::CheckSideL(int row, int column)
{
  int numAround = 0;
  if(previousMap[row-1][column] == '1')
  {
    numAround++;
  }
  if(previousMap[row-1][column+1] == '1')
  {
    numAround++;
  }
  if(previousMap[row][column+1] == '1')
  {
    numAround++;
  }
  if(previousMap[row+1][column+1] == '1')
  {
    numAround++;
  }
  if(previousMap[row+1][column] == '1')
  {
    numAround++;
  }
  return numAround;
}

//counts and returns all of the possible neighbors for the cells in the middle (takes in the position of the cell)
int life::CheckMiddle(int row, int column)
{
  int numAround = 0;
  if(previousMap[row][column-1] == '1')
  {
    numAround++;
  }
  if(previousMap[row][column+1] == '1')
  {
    numAround++;
  }
  if(previousMap[row-1][column-1] == '1')
  {
    numAround++;
  }
  if(previousMap[row-1][column] == '1')
  {
    numAround++;
  }
  if(previousMap[row-1][column+1] == '1')
  {
    numAround++;
  }
  if(previousMap[row+1][column-1] == '1')
  {
    numAround++;
  }
  if(previousMap[row+1][column] == '1')
  {
    numAround++;
  }
  if(previousMap[row+1][column+1] == '1')
  {
    numAround++;
  }
  return numAround;
}

//creates the next generation
void life::NewGen()
{
  for(int i = 0; i < numRows; ++i)
  {
    for(int j = 0; j < numColumns; ++j)
    {
      previousMap[i][j] = currentMap[i][j]; //sets the current map to the previous map
    }
  }

  int numAround; //tracks the number of neighbors for the cell

  for(int i = 0; i < numRows; ++i)
  {
    for(int j = 0; j < numColumns; ++j){
      numAround = 0; //resets the number of neighbors
      //if in row 1
      if(i == 0){
        //if in row 1, column 1
        if(j == 0){
          numAround = CheckCornerTL(i, j);
        }
        //if in row 1, last column
        else if(j == (numColumns - 1)){
          numAround = CheckCornerTR(i, j);
        }
        //anywhere else in row 1
        else
        {
          numAround = CheckSideT(i, j);
        }
      }
      //if in last row
      else if(i == (numRows - 1))
      {
        //if in last row, column 1
        if(j == 0)
        {
          numAround = CheckCornerBL(i, j);
        }
        //if in last row, last column
        else if(j == (numColumns - 1))
        {
          numAround = CheckCornerBR(i, j);
        }
        //anywhere else in the last row
        else
        {
          numAround = CheckSideB(i, j);
        }
      }
      //if column 0
      else if(j == 0)
      {
        //if any other row, column 0
        if(i != 0 && i != (numRows -1))
        {
          numAround = CheckSideL(i, j);
        }
      }
      //if last column
      else if(j == (numColumns - 1))
      {
        //if any other row, last column
        if(i != 0 && i != (numRows -1))
        {
          numAround = CheckSideR(i, j);
        }
      }
      //if it is not on the border anywhere
      else
      {
        numAround = CheckMiddle(i, j);
      }

      //if the cell has one or zero neighbors it dies
      if(numAround <= 1)
      {
        currentMap[i][j] = '-';
      }
      //if the cell has two neighbors it remains the same
      else if(numAround < 3)
      {

      }
      //if the cell has three neighbors it gets filled
      else if(numAround < 4)
      {
        currentMap[i][j] = 'X';
      }
      //if the cell has four, five, six, seven, or eight neighbors it dies
      else
      {
        currentMap[i][j] = '-';
      }
    }
  }
}

//outputs the current map (takes in the file the user wishes to output the results to)
void life::PrintMap(string outputFile)
{
    //opens the file output to the file name of the user's choice
    ofstream outFS;
    outFS.open(outputFile, ios::app);

    //if at the default map
    if(genNum == 0)
    {
      outFS << "0" << endl;
      //prints the map
      genNum++;
      for(int i = 0; i < numRows; ++i)
      {
        for(int j = 0; j < numColumns; ++j)
        {
          outFS << currentMap[i][j];
        }
        outFS << endl;
      }
      outFS << "Generation Number: 1" << endl;
    }
    //if at a generated map
    else
    {
      outFS << "Generation Number: " << genNum << endl;
    }

    //prints the map
    for(int i = 0; i < numRows; ++i)
    {
      for(int j = 0; j < numColumns; ++j)
      {
        outFS << currentMap[i][j];
      }
      outFS << endl;
    }
    genNum++; //increments the generation counter
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
        cout << "invalid arguments (number of threads cannot be lesser than 1)" << endl;
        exit(1);
    }
    cout << "Welcome to Conway's Game of Life!" << endl;
}