#include <unistd.h>
#include <iostream>
#include <fstream>
#include <cmath>
#include <string>
using namespace std;

//class constructor
life::life()
{

}

//class destructor
life::~life()
{

}

/counts and returns all of the possible neighbors for the cell in the top left corner (takes in the position of the cell)
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
      oscillateMap[i][j] = previousMap[i][j]; //sets the previous map to the second previous map
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

//prints the current map (takes in the user's print option choice and the file they wish to output the results to)
void life::PrintMap(string choice, string outputFile)
{
  //if the user wants print option pause
  if(choice == "pause")
  {
    //pauses for .5 seconds every time
    usleep(500000);
    //if this is the original map
    if(genNum == 0)
    {
      cout << "0" << endl;
      //prints the map
      genNum++;
      for(int i = 0; i < numRows; ++i)
      {
        for(int j = 0; j < numColumns; ++j)
        {
          cout << currentMap[i][j];
        }
        cout << endl;
      }
      cout << "Generation Number: 1" << endl;
    }
    //if this is any simulated generation
    else
    {
      cout << "Generation Number: " << genNum << endl;
    }

    //prints the map
    for(int i = 0; i < numRows; ++i)
    {
      for(int j = 0; j < numColumns; ++j)
      {
        cout << currentMap[i][j];
      }
      cout << endl;
    }
    genNum++; //increments the generation counter
  }
  //if the user wants to use the enter print option
  else if(choice == "enter")
  {
    int enterCheck = 0; //checks if the user inputted enter

    //if at the created map
    if(genNum != 0 || genNum != 1)
    {
      //cotninues to check until the user presses enter
      while(enterCheck == 0)
      {
        cout << "Press enter to continue." << endl;
        //if the input is an enter
        if(cin.get() == '\n')
        {
          enterCheck = 1; //exit the while loop
        }
      }
    }

    //if at the created map
    if(genNum == 0)
    {
      cout << "0" << endl;
      //prints the map
      genNum++;
      for(int i = 0; i < numRows; ++i)
      {
        for(int j = 0; j < numColumns; ++j)
        {
          cout << currentMap[i][j];
        }
        cout << endl;
      }
      cout << "Generation Number: 1" << endl;
    }
    //if at a generated map
    else
    {
      cout << "Generation Number: " << genNum << endl;
    }

    //prints the map
    for(int i = 0; i < numRows; ++i)
    {
      for(int j = 0; j < numColumns; ++j)
      {
        cout << currentMap[i][j];
      }
      cout << endl;
    }

    genNum++; //increments the generation counter
  }
  else if(choice == "write")
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
}

class life
{
  public:
    life(); //default constructor
    ~life(); //destructor

    void RandomMap(int rows, int columns, float cellFreq); //creates a map for a random gen
    void GivenMap(int rows, int columns, string fileName); //creates a map for a given gen

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
    void PrintMap(string choice, string outputFile); //prints the current map
    bool CheckValid(); //checks if the generations should continue

    char** oscillateMap; //map before the previous map
    char** previousMap; //old map
    char** currentMap; //new map
};

int main(int argc, char** argv)
{
    string inputFile = argv[1];
    string outputFile = argv[2];
    int numSteps = argv[3];
    int numThreads = argv[4];


    cout << "Welcome to Conway's Game of Life!" << endl;
}