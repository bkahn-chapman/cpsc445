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
    int startingPt;
    string dna; // initial string
    vector<char> atgc; // vector of all characters in initial string
    vector<int> counts;
    void readString(int numThreads);
    void calcCounts(int rank, int numThreads);
    void sendCounts();
    void printResults(int rank);
    void check_error(int status, const string message);
};

void Test::readString(int numThreads) {
  ifstream inFS;
  inFS.open("dna.txt");
  getline(inFS, dna);
  inFS.close();
  int length = dna.length();
  for(int i = 0; i < length; ++i)
  {
    atgc.push_back(dna[i]);
  }
  MPI_Scatter(&atgc[0], atgc.size(), MPI_CHAR, &atgc[1], atgc.size(), MPI_CHAR, 0, MPI_COMM_WORLD);
}

void Test::calcCounts(int rank, int numThreads) {
  int startingPt = rank * 4;
  for(int i = 0; i < atgc.size(); ++i)
  {
    if(atgc.size() % numThreads == rank)
    {
      if(atgc[i] != 'A' && atgc[i] != 'T' && atgc[i] != 'G' && atgc[i] != 'C')
      {
          cout << "invalid input (characters other than A, T, G, and C are not allowed)" << endl;
          exit(1);
      }
      if(atgc[i] == 'A')
      {
        cout << "A";
        countA++;
      }
      else if(atgc[i] == 'T')
      {
        cout << "T";
        countT++;
      }
      else if(atgc[i] == 'G')
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
  counts[startingPt] = countA;
  counts[startingPt+1] = countT;  
  counts[startingPt+2] = countG;
  counts[startingPt+3] = countC;
}

void Test::printResults(int rank)
{
  MPI_Gather(&counts[0], counts.size(), MPI_INT, &counts[1], counts.size(), MPI_INT, 0, MPI_COMM_WORLD);
  ofstream outFS;
  outFS.open("output.txt");
  outFS << "A " << countA << "\nT " << countT << "\nG " << countG << "\nC " << countC;
  outFS.close();
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

  // check_error(MPI_Finalize());
  Test t;
  if(rank == 0)
  {
    t.readString(p);
  }
  t.calcCounts(rank, p);
  t.printResults(rank);
  // cout << "Ending process " << rank << "/" << "p\n";
  MPI_Finalize();
  return 0;
}
