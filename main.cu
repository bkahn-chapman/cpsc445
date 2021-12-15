#include <string>
#include <fstream>
#include <iostream>
#include <vector>
using namespace std;

__global__
void find_corners(string *a, int *b, int N)
{
    
}

int main () {
    ifstream inFS;
    inFS.open("input.txt");
    vector<double> max_min;
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
        for(int i = 0; i < line.length()-1; ++i)
        {
            if(line[i] == '(')
            {
                for(int t = i; t < line.length()-1; ++t)
                {
                    if(line[t] == ',')
                    {
                        if(checkFirstX == 0)
                        {
                            max_x = stod(numX);
                            min_x = stod(numX);
                            checkFirstX = 1;
                        }
                        else
                        {
                            test = stod(numX);
                            if(test > max_x)
                            {
                                max_x = test;
                            }
                            else if(test < min_x)
                            {
                                min_x = test;
                            }
                        }
                        numX = "";
                        XorY = 1;
                    }
                    else if(line[t] == ')')
                    {
                        if(checkFirstY == 0)
                        {
                            max_y = stod(numY);
                            min_y = stod(numY);
                            checkFirstY = 1;
                        }
                        else
                        {
                            testY = stod(numY);
                            if(testY > max_y)
                            {
                                max_y = testY;
                            }
                            else if(testY < min_y)
                            {
                                min_y = testY;
                            }
                        }
                        numY = "";
                        XorY = 0;
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
    }
}