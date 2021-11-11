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
    vector<int> sepcountstrip;
    vector<int> sepcounts;
    vector<int> combtriplets;
    vector<int> combcounts;
    vector<int> finaltrips;
    vector<int> finalcounts;
    vector<string> tripletters;
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
    if(sepcountstrip.size() == 0)
    {
      sepcountstrip.push_back(septriplets[i]);
      sepcounts.push_back(1);
    }
    else
    {
      for(int j = 0; j < sepcountstrip.size(); ++j)
      {
        if(septriplets[i] == sepcountstrip[j])
        {
          sepcounts[j]++;
          break;
        }
        sepcountstrip.push_back(septriplets[i]);
        sepcounts.push_back(1);
      }
    }
  }
  if(rank == 0)
  {
    combtriplets.resize(dnaLength);
    combcounts.resize(dnaLength);
  }
  MPI_Gather(&sepcountstrip[0], dnaSize, MPI_INT, &combtriplets[0], dnaSize, MPI_INT, 0, MPI_COMM_WORLD);
  MPI_Gather(&sepcounts[0], dnaSize, MPI_INT, &combcounts[0], dnaSize, MPI_INT, 0, MPI_COMM_WORLD);
  if(rank == 0)
  {
    for(int i = 0; i < combtriplets.size(); ++i)
    {
      if(combtriplets[i] != -1)
      {
        if(i == 0)
        {
          finaltrips.push_back(combtriplets[0]);
          finalcounts.push_back(combcounts[0]);
        }
        else
        {
          vector<int>::iterator test;
          test = find (finaltrips.begin(), finaltrips.end(), combtriplets[i]);
          if (test != finaltrips.end())
          {
            for(int j = 0; j < finaltrips.size(); ++j)
            {
              if(finaltrips[j] == combtriplets[i])
              {
                finalcounts[j]++;
              }
            }
          }
          else
          {
            finaltrips.push_back(combtriplets[i]);
            finalcounts.push_back(combcounts[i]);
          }
        }
      }
    }
    char letts[] = {'A', 'T', 'G', 'C'};
    for(int f = 0; f < finaltrips.size(); ++f)
    {
      int countf = 0;
      string finaltest = "";
      for(int i = 0; i < 4; ++i)
      {
        for(int j = 0; j < 4; ++j)
        {
          for(int k = 0; k < 4; ++k)
          {
            finaltest = "";
            finaltest.push_back(letts[i]);
            finaltest.push_back(letts[j]);
            finaltest.push_back(letts[k]);
            if(countf == finaltrips[f])
            {
              tripletters.push_back(finaltest);
            }
            countf++;
          }
        }
      }
    }
  }
}

void Count::outputResults(int rank) {
  if(rank == 0)
  {
    ofstream outFS;
    outFS.open("output.txt");
    for(int i = 0; i < tripletters.size(); ++i)
    {
      outFS << tripletters[i] << " " << finalcounts[i] << endl;
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
