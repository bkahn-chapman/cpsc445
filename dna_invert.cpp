#include <mpi.h>
#include <iostream>
#include <vector>
#include <fstream>
#include <stdio.h>
#include <cmath>
#include <array>
using namespace std;

class Count {
  public:
    void makeVector(int rank, int p);
    void check_error(int status, const string message);
    void spreadValues(int rank, int p);
    void calcInverse(int rank, int p);
    void outputResults(int rank);
    int dnaSize;
    int dnaLength;
    vector<char> initial;
    vector<char> received;
    vector<char> finalchars;
};

void Count::makeVector(int rank, int p) {
  if(rank == 0)
  {
    ifstream inFS;
    inFS.open("dna.txt");
    char c;
    while(inFS >> noskipws >> c)
    {
      initial.push_back(c);
    }
    inFS.close();
    dnaSize = (initial.size() / p) +1;
    dnaLength = initial.size();
  }
}

void Count::spreadValues(int rank, int p) {
  MPI_Bcast(&dnaSize, 1, MPI_INT, 0, MPI_COMM_WORLD);
  MPI_Bcast(&dnaLength, 1, MPI_INT, 0, MPI_COMM_WORLD);
  received.resize(dnaSize);
  MPI_Scatter(&initial[0], dnaSize, MPI_CHAR, &received[0], dnaSize, MPI_CHAR, 0, MPI_COMM_WORLD);
}

void Count::calcInverse(int rank, int p) {
  for(int i = 0; i < received.size(); ++i)
  {
    if(received[i] == 'A')
    {
      received[i] = 'T';
    }
    if(received[i] == 'T')
    {
      received[i] = 'A';
    }
    if(received[i] == 'G')
    {
      received[i] = 'C';
    }
    if(received[i] == 'C')
    {
      received[i] = 'G';
    }
  }
  if(rank == 0)
  {
    finalchars.resize(dnaLength);
  }
  MPI_Gather(&received[0], dnaSize, MPI_CHAR, &finalchars[0], dnaSize, MPI_CHAR, 0, MPI_COMM_WORLD);
}

void Count::outputResults(int rank) {
  if(rank == 0)
  {
    ofstream outFS;
    outFS.open("output.txt");
    for(int i = 0; i < finalchars.size(); ++i)
    {
      outFS << finalchars[i];
    }
    outFS.close();
  }
}

void Count::check_error(int status, const string message="MPI error") {
  if ( status != 0 ) {    
    cerr << "Error: " << message << endl;
    exit(1);
  }
}

int main (int argc, char *argv[]) {
  int rank;
  int p;
  
  Count count;
  // Initialized MPI
  count.check_error(MPI_Init(&argc, &argv), "unable to initialize MPI");
  count.check_error(MPI_Comm_size(MPI_COMM_WORLD, &p), "unable to obtain p");
  count.check_error(MPI_Comm_rank(MPI_COMM_WORLD, &rank), "unable to obtain rank");

  count.makeVector(rank, p);
  count.spreadValues(rank, p);
  count.calcInverse(rank, p);
  count.outputResults(rank);

  count.check_error(MPI_Finalize());

  return 0;
}
