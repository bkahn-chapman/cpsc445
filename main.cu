#include <string>
#include <fstream>
#include <iostream>
#include <vector>
using namespace std;

__global__
void getOverlaps(double *m, double *s, double *n, double *o, int S)
{

}

int main () {
    ifstream inFS;
    inFS.open("input.txt");
    vector<double> max_min;
    vector<double> sizes;
    vector<double> nums;
    vector<double> overlaps;
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
    int M = max_min.size();
    int N = nums.size();
    int S = sizes.size();
    int p = 0;
    for(int i = 1; i < S; ++i)
    {
        p = p + i;
    }
    cout << p << endl;
    int O = overlaps.size();
    double hm[M], hn[N], hs[S], hO[O];
    double *dm, *dn, *ds, *dO;
    cudaMalloc((void **)&dm, N*sizeof(double));
    cudaMalloc((void **)&dn, N*sizeof(double));
    cudaMalloc((void **)&ds, N*sizeof(double));
    cudaMalloc((void **)&dO, N*sizeof(double));
    for (int i = 0; i<N; ++i) {
        hn[i] = nums[i];
    }
    for (int i = 0; i<M; ++i) {
        hm[i] = max_min[i];
    }
    for (int i = 0; i<S; ++i) {
        hs[i] = sizes[i];
    }
    for (int i = 0; i<O; ++i) {
        hO[i] = overlaps[i];
    }
    cudaMemcpy(dm, hm, M*sizeof(double), cudaMemcpyHostToDevice);
    cudaMemcpy(dn, hn, N*sizeof(double), cudaMemcpyHostToDevice);
    cudaMemcpy(ds, hs, S*sizeof(double), cudaMemcpyHostToDevice);
    getOverlaps<<<S, 1>>>(dm, ds, dn, S);
}
