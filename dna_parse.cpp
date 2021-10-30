#include <mpi.h>
#include <iostream>
#include <vector>
#include <fstream>
#include <stdio.h>
#include <cmath>
#include <array>
#include <string>
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
    vector<string> received;
    vector<int> septriplets;
    vector<int> sepcounts;
    vector<int> finaltriplets;
    vector<int> finalcounts;
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
    string s = "";
    for(int i = 0; i < initial.size(); ++i)
    {
      if(initial[i]=='A')
      {
        initial[i] = '1';
      }
      if(initial[i]=='T')
      {
        initial[i] = '2';
      }
      if(initial[i]=='G')
      {
        initial[i] = '3';
      }
      if(initial[i]=='C')
      {
        initial[i] = '4';
      }
    }
    for(int i = 0; i < initial.size(); ++i)
    {
      s.push_back(initial[i]);
      if(i % 3 == 2)
      {
        triplets.push_back(s);
        s = "";
      }
    }
    dnaSize = (triplets.size() / p) +1;
    dnaLength = triplets.size();
  }
}

void Count::spreadValues(int rank, int p) {
  MPI_Bcast(&dnaSize, 1, MPI_INT, 0, MPI_COMM_WORLD);
  MPI_Bcast(&dnaLength, 1, MPI_INT, 0, MPI_COMM_WORLD);
  received.resize(dnaSize);
  MPI_Scatter(&triplets[0], dnaSize, MPI_CHAR, &received[0], dnaSize, MPI_CHAR, 0, MPI_COMM_WORLD);
}

void Count::calcTotals(int rank, int p) {
  vector<vector<int>> totals;
  for(int i = 0; i < received.size(); ++i)
  {
    if(i == 0)
    {
      totals[0][0] = stoi(received[i]);
      totals[0][1] = 1;
    }
    bool newCheck = false;
    for(int j = 0; j < totals.size(); ++j)
    {
      if(stoi(received[i])==totals[i][0])
      {
        newCheck = true;
        totals[i][1]++;
        break;
      }
    }
    if(newCheck == false)
    {
      totals[totals.size()][0] = stoi(received[i]);
      totals[totals.size()][1] = 1;
    }
  }
  for(int i = 0; i < totals.size(); ++i)
  {
    septriplets.push_back(totals[i][0]);
    sepcounts.push_back(totals[i][1]);
  }
  if(rank == 0)
  {
    finaltriplets.resize(dnaSize);
    finalcounts.resize(dnaSize);
  }
  MPI_Gather(&septriplets[0], dnaSize, MPI_INT, &finaltriplets[0], dnaSize, MPI_INT, 0, MPI_COMM_WORLD);
  MPI_Gather(&sepcounts[0], dnaSize, MPI_INT, &finalcounts[0], dnaSize, MPI_INT, 0, MPI_COMM_WORLD);
}

void Count::outputResults(int rank) {
  if(rank == 0)
  {
    ofstream outFS;
    outFS.open("output.txt");
    for(int i = 0; i < finaltriplets.size(); ++i)
    {
      outFS << finaltriplets[i] << " " << finalcounts[i] << endl;
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
  count.calcTotals(rank, p);
  count.outputResults(rank);

  count.check_error(MPI_Finalize());

  return 0;
}
