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
    int dnaSize;
    vector<char> initial;
    vector<char> received;
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
  }
}

void Count::spreadValues(int rank, int p) {
  MPI_Bcast(&dnaSize, 1, MPI_INT, 0, MPI_COMM_WORLD);
  received.resize(dnaSize);
  MPI_Scatter(&initial[0], dnaSize, MPI_CHAR, &received[0], dnaSize, MPI_CHAR, 0, MPI_COMM_WORLD);
}

void Count::calcTotals(int rank, int p) {
  for(int i = 0; i < received.size(); ++i)
  {
    if(received[i] == 'A')
    {
      cout << "rank: " << rank << " " << received[i] << endl;
    }
    if(received[i] == 'T')
    {
      cout << "rank: " << rank << " " << received[i] << endl;
    }
    if(received[i] == 'G')
    {
      cout << "rank: " << rank << " " << received[i] << endl;
    }
    if(received[i] == 'C')
    {
      cout << "rank: " << rank << " " << received[i] << endl;
    }
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

  count.check_error(MPI_Finalize());

  return 0;
}
