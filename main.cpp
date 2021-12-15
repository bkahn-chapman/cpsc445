#include <mpi.h>
#include <string>
#include <fstream>
#include <iostream>
#include <vector>
using namespace std;

class Overlap {
    public:
        void check_error(int status, const string message);
        void makeVectors(int rank);
        void spreadValues(int rank, int p);
        vector<double> max_min;
        vector<double> sizes;
        vector<double> nums;
        vector<double> overlaps;
};

void Overlap::check_error(int status, const string message="MPI error") {
  if ( status != 0 ) {    
    cerr << "Error: " << message << endl;
    exit(1);
  }
}

void Overlap::makeVectors(int rank) {
    if(rank == 0)
    {
        ifstream inFS;
        inFS.open("input.txt");
        string line;
        while(getline(inFS, line))
        {
            string numX;
            string numY;
            double max_x = 0;
            double min_x = 0;
            double max_y = 0;
            double min_y = 0;
            int checkFirstX = 0;
            int checkFirstY = 0;
            int XorY = 0;
            int pairs = 0;
            for(int i = 0; i < line.length()-1; ++i)
            {
                if(line[i] == '(')
                {
                    for(int t = i+1; t < line.length(); ++t)
                    {
                        if(line[t] == ',')
                        {
                            if(checkFirstX == 0)
                            {
                                max_x = atof(numX.c_str());
                                min_x = atof(numX.c_str());
                                nums.push_back(max_x);
                                checkFirstX = 1;
                            }
                            else
                            {
                                double testX = atof(numX.c_str());
                                if(testX > max_x)
                                {
                                    max_x = testX;
                                }
                                else if(testX < min_x)
                                {
                                    min_x = testX;
                                }
                                nums.push_back(testX);
                            }
                            numX = "";
                            XorY = 1;
                        }
                        else if(line[t] == ')')
                        {
                            if(checkFirstY == 0)
                            {
                                max_y = atof(numY.c_str());
                                min_y = atof(numY.c_str());
                                nums.push_back(max_y);
                                checkFirstY = 1;
                            }
                            else
                            {
                                double testY = atof(numY.c_str());;
                                if(testY > max_y)
                                {
                                    max_y = testY;
                                }
                                else if(testY < min_y)
                                {
                                    min_y = testY;
                                }
                                nums.push_back(testY);
                            }
                            numY = "";
                            XorY = 0;
                            pairs++;
                            break;
                        }
                        else
                        {
                            if(XorY == 0)
                            {
                                numX.push_back(line[t]);
                            }
                            else
                            {
                                numY.push_back(line[t]);
                            }
                        }
                    }
                }
            }
            sizes.push_back(pairs);
            max_min.push_back(max_x);
            max_min.push_back(min_x);
            max_min.push_back(max_y);
            max_min.push_back(min_y);
        }
        inFS.close();
        for(int i = 0; i < sizes.size(); ++i)
        {
            cout << sizes[i] << endl;
        }
    }
}

void Overlap::spreadValues(int rank, int p) {
    
}

int main (int argc, char *argv[]) {
    int rank;
    int p;

    Overlap over;
    // Initialized MPI
    over.check_error(MPI_Init(&argc, &argv), "unable to initialize MPI");
    over.check_error(MPI_Comm_size(MPI_COMM_WORLD, &p), "unable to obtain p");
    over.check_error(MPI_Comm_rank(MPI_COMM_WORLD, &rank), "unable to obtain rank");

    over.makeVectors(rank);

}
