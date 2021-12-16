#include <string>
#include <fstream>
#include <iostream>
#include <vector>
using namespace std;

//the function to find the overlaps
//COULD NOT GET THIS TO WORK WITH THE SETUP I CREATED
//plan was to have one shape at a time check for overlaps
//it would check if any point in the shape is contained in the min/max square of the other shapes
//if there was an overlap, add the shapes to the overlap array (ex. 1 2 0 0 1 4 0 0 2 3 0 0 0 0 0 0 3 5 4 5)
//then return that array for main to handle the logic of the array and output results
__global__
void getOverlaps(double *m, double *s, double *n, double *o, int M, int N, int S, int O)
{
    int i = blockIdx.x;
    if(i < N)
    {
        o[i] = 1;
    }
}

int main () {
    ifstream inFS;
    inFS.open("input.txt");
    vector<double> max_min; //each row's max and min X and Y values
    vector<double> sizes; //the number of points in each shape
    vector<double> nums; //every single point of every single shape
    vector<double> overlaps; //contains a variable for every potential overlap
    string line; //each line of the file
    while(getline(inFS, line))
    {
        string numX; //the point's X value
        string numY; //the point's Y value
        double max_x = 0; //the shape's max X
        double min_x = 0; //the shape's min X
        double max_y = 0; //the shape's max Y
        double min_y = 0; //the shape's min Y
        int checkFirstX = 0; //checks if the point is the first in the shape
        int checkFirstY = 0; //checks if the point is the first in the shape
        int XorY = 0; //checks if it is the X variable or the Y variable
        int pairs = 0; //tracks how many points are in each shape
        for(int i = 0; i < line.length()-1; ++i) //checks each point in the shape
        {
            if(line[i] == '(') //if it's the start of the point
            {
                for(int t = i+1; t < line.length(); ++t) //each char in the point
                {
                    if(line[t] == ',') //if the X variable of the point is finished
                    {
                        if(checkFirstX == 0) //if the first point in the shape
                        {
                            max_x = atof(numX.c_str()); //sets max X
                            min_x = atof(numX.c_str()); //sets min X
                            nums.push_back(max_x); //adds the X variable to the list
                            checkFirstX = 1; //ends the check
                        }
                        else
                        {
                            double testX = atof(numX.c_str()); //the X variable that isn't from the first point
                            if(testX > max_x) //if higher than the current max
                            {
                                max_x = testX; //new max
                            }
                            else if(testX < min_x) //if lower than the current min
                            {
                                min_x = testX; //new min
                            }
                            nums.push_back(testX); //adds the X variable to the list
                        }
                        numX = ""; //resets the number string
                        XorY = 1; //next number will be a Y
                    }
                    else if(line[t] == ')') //if the Y variable of the point is finished
                    {
                        if(checkFirstY == 0) //if the first point in the shape
                        {
                            max_y = atof(numY.c_str()); //sets max Y
                            min_y = atof(numY.c_str()); //sets min Y
                            nums.push_back(max_y); //adds the Y variable to the list
                            checkFirstY = 1; //ends the check
                        }
                        else
                        {
                            double testY = atof(numY.c_str());; //the Y variable that isn't from the first point
                            if(testY > max_y) //if higher than the current max
                            {
                                max_y = testY; //new max
                            }
                            else if(testY < min_y) //if lower than the current min
                            {
                                min_y = testY; //new min
                            }
                            nums.push_back(testY); //adds the Y variable to the list
                        }
                        numY = ""; //resets the number string
                        XorY = 0; //next number will be an X
                        pairs++; //counts that another point has been finished
                        break; //ends the loop for the point
                    }
                    else //if there is still more to the variable
                    {
                        if(XorY == 0) //if X variable
                        {
                            numX.push_back(line[t]); //add the next char to the X variable
                        }
                        else //if Y variable
                        {
                            numY.push_back(line[t]); //add the next char to the Y variable
                        }
                    }
                }
            }
        }
        sizes.push_back(pairs); //adds the number of points in the shape to the list
        max_min.push_back(max_x); //adds the max X of the shape
        max_min.push_back(min_x); //adds the min X of the shape
        max_min.push_back(max_y); //adds the max Y of the shape
        max_min.push_back(min_y); //adds the min Y of the shape
    }
    inFS.close();
    int M = max_min.size(); //the total number of max/min
    int N = nums.size(); //the total number of points * 2
    int S = sizes.size(); //the total number of shapes
    //cout << M << endl;
    //cout << N << endl;
    //cout << S << endl;
    int p = 0; //number of possible overlap pairings
    for(int i = 1; i < S; ++i) //based on the number of shapes
    {
        p = p + i; //(shapes - 1) + (shapes - 2)... + 1
    }
    int O = p*2; //number of possible overlaps * 2 (one slot for each shape in the overlap)
    double hm[M], hn[N], hs[S], hO[O]; //arrays for each vector (m = min-max, n = nums, s = sizes, o =overlaps)
    double *dm, *dn, *ds, *dO; //pointers for each array 
    cudaMalloc((void **)&dm, M*sizeof(double)); //size of min-max array
    cudaMalloc((void **)&dn, N*sizeof(double)); //size of nums array
    cudaMalloc((void **)&ds, S*sizeof(double)); //size of sizes array
    cudaMalloc((void **)&dO, O*sizeof(double)); //size of overlaps array
    //fill the nums array
    for (int i = 0; i<N; ++i) {
        hn[i] = nums[i];
    }
    //fill the min-max array
    for (int i = 0; i<M; ++i) {
        hm[i] = max_min[i];
    }
    //fill the sizes array
    for (int i = 0; i<S; ++i) {
        hs[i] = sizes[i];
    }
    //fill the overlaps array
    for (int i = 0; i<O; ++i) {
        hO[i] = 0;
    }
    cudaMemcpy(dm, hm, M*sizeof(double), cudaMemcpyHostToDevice); //sends min-max array to device
    cudaMemcpy(dn, hn, N*sizeof(double), cudaMemcpyHostToDevice); //sends nums array to device
    cudaMemcpy(ds, hs, S*sizeof(double), cudaMemcpyHostToDevice); //sends sizes array to device
    cudaMemcpy(dO, hO, O*sizeof(double), cudaMemcpyHostToDevice); //sends overlaps array to device
    getOverlaps<<<N, 1>>>(dm, ds, dn, dO, M, S, N, O); //the global function to find the overlaps
    cudaMemcpy(hO, dO, O*sizeof(double), cudaMemcpyDeviceToHost); //receives the overlaps array back to the host
    ofstream outFS;
    outFS.open("output.txt")
    for(int i = 0; i < O; ++i) //iterates through the entire received overlaps array
    {
        if(hO[i] != 0) //if the overlap checker isn't empty
        {
            if(i % 2 == 0) //if the first of a pair of shapes (0 to 1, 4 to 5, etc.)
            {
                outFS << "Shape: " << ho[i] << " overlaps with ";
            }
            else //if the second of a pair of shapes (1 with 0, 5 with 4, etc.)
            {
                outFS << "shape: " << ho[i] << "." << endl;
            }
        }

    }
}
