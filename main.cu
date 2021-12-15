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
    vector<double> sizes;
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
                        }
                        numX = "";
                        XorY = 1;
                    }
                    else if(line[t] == ')')
                    {
                        if(checkFirstY == 0)
                        {
                            max_y = atof(numY.c_str());;
                            min_y = atof(numY.c_str());;
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
    for(int i = 0; i < sizes.size(); ++i)
    {
        cout << sizes[i] << endl;
    }
}