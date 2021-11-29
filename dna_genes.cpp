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
    int totalCorrect;
    vector<char> initial;
    vector<string> triplets;
    vector<int> converted;
    vector<int> septriplets;
    vector<int> sepstarts;
    vector<int> sepends;
    vector<int> starts;
    vector<int> ends;
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
    char letters[] = {'A', 'C', 'G', 'T'};
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
  char letts[] = {'A', 'C', 'G', 'T'};
  string trips = "";
  int first = rank * septriplets.size();
  int count = 0;
  int corrects = 0;
  int start = 0;
  int end1 = 0;
  int end2 = 0;
  int end3 = 0;
  for(int i = 0; i < 4; ++i)
  {
    for(int j = 0; j < 4; ++j)
    {
      for(int k = 0; k < 4; ++k)
      {
        trips = "";
        trips.push_back(letts[i]);
        trips.push_back(letts[j]);
        trips.push_back(letts[k]);
        if(trips == "ATG")
        {
          start = count;
          corrects++;
        }
        else if (trips == "TGA")
        {
          end1 = count;
          corrects++;
        }
        else if (trips == "TAG")
        {
          end2 = count;
          corrects++;
        }
        else if (trips == "TAA")
        {
          end3 = count;
          corrects++;
        }
        count++;
      }
    }
  }
  for(int i = 0; i < septriplets.size(); ++i)
  {
    if(septriplets[i] == start)
    {
      sepstarts.push_back(first + i);
    }
    else if(septriplets[i] == end1 || septriplets[i] == end2 || septriplets[i] == end3)
    {
      sepends.push_back(first + i);
    }
  }
  for(int i = 0; i < sepstarts.size(); ++i)
  {
    cout << rank << ": " << sepstarts[i] << endl;
  }
}

void Count::outputResults(int rank) {
  if(rank == 0)
  {
    ofstream outFS;
    outFS.open("output.txt");
    char lett[] = {'A', 'C', 'G', 'T'};
    string trip = "";
    int count = 0;
    for(int i = 0; i < 4; ++i)
    {
      for(int j = 0; j < 4; ++j)
      {
        for(int k = 0; k < 4; ++k)
        {
          trip = "";
          trip.push_back(lett[i]);
          trip.push_back(lett[j]);
          trip.push_back(lett[k]);
          if(combtotals[count] > 0)
          {
            outFS << trip << " " << combtotals[count] << endl;
          }
          count++;
        }
      }
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
