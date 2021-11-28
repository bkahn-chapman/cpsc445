#include <mpi.h>
#include <iostream>
#include <vector>
#include <fstream>
#include <stdio.h>
#include <cmath>
#include <array>
#include <string>
#include <algorithm>
using namespace std;

class Count {
  public:
    void makeVector(int rank, int p);
    void check_error(int status, const string message);
    void spreadValues(int rank, int p);
    void calcTotals(int rank, int p);
    void outputResults(int rank);
    int dnaSize;
    int dnaLength;
    vector<char> initial;
    vector<string> triplets;
    vector<int> converted;
    vector<int> septriplets;

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
    string triplet = "";
    for(int i = 1; i < initial.size()+1; ++i)
    {
      triplet.push_back(initial[i-1]);
      if(i % 3 == 0)
      {
        triplets.push_back(triplet);
        triplet = "";
      }
    }
    string tripTest = "";
    char letters[] = {'A', 'T', 'G', 'C'};
    for(int t = 0; t < triplets.size(); ++t)
    {
      int count = 0;
      for(int i = 0; i < 4; ++i)
      {
        for(int j = 0; j < 4; ++j)
        {
          for(int k = 0; k < 4; ++k)
          {
            tripTest = "";
            tripTest.push_back(letters[i]);
            tripTest.push_back(letters[j]);
            tripTest.push_back(letters[k]);
            if(tripTest == triplets[t])
            {
              converted.push_back(count);
            }
            count++;
          }
        }
      }
    }
    if(converted.size() % p != 0)
    {
      int toAdd = p - (converted.size() % p);
      if(toAdd != 0)
      {
        for(int i = 0; i < toAdd; ++i)
        {
          converted.push_back(-1);
        }
      }
    }
    dnaSize = converted.size() / p;
    dnaLength = converted.size();
  }
}

void Count::spreadValues(int rank, int p) {
  MPI_Bcast(&dnaSize, 1, MPI_INT, 0, MPI_COMM_WORLD);
  MPI_Bcast(&dnaLength, 1, MPI_INT, 0, MPI_COMM_WORLD);
  septriplets.resize(dnaSize);
  MPI_Scatter(&converted[0], dnaSize, MPI_INT, &septriplets[0], dnaSize, MPI_INT, 0, MPI_COMM_WORLD);
}

void Count::calcTotals(int rank, int p) {
  for(int i = 0; i < septriplets.size(); ++i)
  {
    cout << rank << ", " << septriplets[i] << endl;
  }
}

void Count::outputResults(int rank) {
  
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
