#include <mpi.h>
#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <vector>

using namespace std;

class Test
{
  public:
    int countA;
    int countT;
    int countG;
    int countC;
    string dna;
    vector<char> atgc;
    void getString(int rank, int numThreads);
    void calcCounts(int rank, int numThreads);
    void printResults(int rank);
};

void Test::getString(int rank, int numThreads) {
  if(rank == 0)
  {
    ifstream inFS;
    inFS.open("dna.txt");
    getline(inFS, dna);
    inFS.close();
    int length = dna.length();
    for(int i = 0; i < length; ++i)
    {
      atgc.push_back(dna[i]);
    }
    for(int i = 1; i < numThreads; ++i)
    {
      MPI_Send(&atgc, atgc.size(), MPI_CHAR, i, 0, MPI_COMM_WORLD);
      cout << "Sent to rank: " << i << endl;
    }
  }
  else
  {
    MPI_Recv(&atgc, atgc.size(), MPI_CHAR, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    for(int i = 0; i < atgc.size(); ++ i)
    {
      cout << "Rank: " << rank << " received: " << atgc[i];
    }
  }
  MPI_Finalize();
}

void Test::calcCounts(int rank, int numThreads) {
  for(int i = 0; i < dna.length(); ++i)
  {
    if(dna.length() % numThreads == rank)
    {
      if(dna[i] != 'A' && dna[i] != 'T' && dna[i] != 'G' && dna[i] != 'C')
      {
          cout << "invalid input (characters other than A, T, G, and C are not allowed)" << endl;
          exit(1);
      }
      if(dna[i] == 'A')
      {
        cout << "A";
        countA++;
      }
      else if(dna[i] == 'T')
      {
        cout << "T";
        countT++;
      }
      else if(dna[i] == 'G')
      {
        cout << "G";
        countG++;
      }
      else
      {
        cout << "C";
        countC++;
      }
    }
  }
}

void Test::printResults(int rank)
{
  if(rank != 0)
  {
    
  }
  else
  {
    ofstream outFS;
    outFS.open("output.txt");
    outFS << "A " << countA << "\nT " << countT << "\nG " << countG << "\nC " << countC;
    outFS.close();
  }
}

void check_error(int status, const string message="MPI error") {
  if ( status != 0 ) {    
    cerr << "Error: " << message << endl;
    exit(1);
  }
}

int main (int argc, char *argv[]) {
  int rank;
  int p;

  // Initialized MPI
  check_error(MPI_Init(&argc, &argv), "unable to initialize MPI");
  check_error(MPI_Comm_size(MPI_COMM_WORLD, &p), "unable to obtain p");
  check_error(MPI_Comm_rank(MPI_COMM_WORLD, &rank), "unable to obtain rank");
  // cout << "Starting process " << rank << "/" << "p\n";

  // example code
  // int n = (rank==0?5:0), sum = 0;
  // check_error(MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD));  
  // check_error(MPI_Reduce(&n, &sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD));
  /*
  if (rank==0) {
    if (sum != n*p) { cerr << "error!\n"; exit(1); }
  }
  */

  check_error(MPI_Finalize());
  Test t;
  t.getString(rank, p);
  t.calcCounts(rank, p);
  t.printResults(rank);
  // cout << "Ending process " << rank << "/" << "p\n";

  return 0;
}
