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
    void calcTotals(int rank, int p);
    void outputResults(int rank);
    int dnaSize;
    vector<char> initial;
    vector<char> received;
    vector<int> finaltotals;
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
    //make initial to a size where it is cleanly divisible by p
    inFS.close();
    int toAdd = p - (initial.size() % p);
    if(toAdd != 0)
    {
      for(int i = 0; i < toAdd; ++i)
      {
        initial.push_back(' ');
      }
    }
    dnaSize = (initial.size() / p);
  }
}

void Count::spreadValues(int rank, int p) {
  MPI_Bcast(&dnaSize, 1, MPI_INT, 0, MPI_COMM_WORLD);
  received.resize(dnaSize);
  MPI_Scatter(&initial[0], dnaSize, MPI_CHAR, &received[0], dnaSize, MPI_CHAR, 0, MPI_COMM_WORLD);
}

void Count::calcTotals(int rank, int p) {
  vector<int> totals = {0, 0, 0, 0};
  for(int i = 0; i < received.size(); ++i)
  {
    if(received[i] == 'A')
    {
      totals[0]++;
    }
    if(received[i] == 'T')
    {
      totals[1]++;
    }
    if(received[i] == 'G')
    {
      totals[2]++;
    }
    if(received[i] == 'C')
    {
      totals[3]++;
    }
  }
  if(rank == 0)
  {
    finaltotals.resize(4);
  }
  MPI_Reduce(&totals[0], &finaltotals[0], 4, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD); //make reduce
}

void Count::outputResults(int rank) {
  if(rank == 0)
  {
    ofstream outFS;
    outFS.open("output.txt");
    outFS << "A: " << finaltotals[0] << endl;
    outFS << "T: " << finaltotals[1] << endl;
    outFS << "G: " << finaltotals[2] << endl;
    outFS << "C: " << finaltotals[3] << endl;
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
  count.calcTotals(rank, p);
  count.outputResults(rank);

  count.check_error(MPI_Finalize());

  return 0;
}
