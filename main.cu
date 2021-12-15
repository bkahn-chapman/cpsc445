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
                    if(t[i] == ',')
                    {
                        if(checkFirstX == 0)
                        {
                            max_x = stod(numX);
                            min_x = stod(numX);
                            checkFirstX = 1;
                        }
                        else
                        {
                            if(stod(numX) > max_x)
                            {
                                max_x = stod(numX);
                            }
                            else if(stod(numX) < min_x))
                            {
                                min_x = stod(numX);
                            }
                        }
                        numX = "";
                        XorY = 1;
                    }
                    else if(t[i] == ')')
                    {
                        if(checkFirstY == 0)
                        {
                            max_y = stod(numY);
                            min_y = stod(numY);
                            checkFirstY = 1;
                        }
                        else
                        {
                            if(stod(numY) > max_y)
                            {
                                max_y = stod(numY);
                            }
                            else if(stod(numY) < min_y))
                            {
                                min_y = stod(numY);
                            }
                        }
                        numY = "";
                        XorY = 0;
                    }
                    else
                    {
                        if(XorY == 0)
                        {
                            numX.append(t[i]);
                        }
                        else
                        {
                            numY.append(t[i]);
                        }
                    }
                }
            }
        }
    }
}