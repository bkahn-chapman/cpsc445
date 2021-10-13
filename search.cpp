#include <fstream>
#include <iostream>
#include <string>
#include <thread>
#include <vector>
using namespace std;

vector<int> wordCounter(int numThreads, int threadNum, vector<string> words, vector<string> sentences)
{
    vector<int> counts;
    for(int s = 0; s < sentences.size(); ++s)
    {
        if(s % numThreads == threadNum)
        {
            for(int w = 0; w < words.size(); ++w)
            {
                counts.push_back(w);
            }
        }
    }
    return counts;
}

void checkArgs(int numArgs)
{
    if((numArgs-1) != 4)
    {
        cout << "invalid arguemnts (expected 4 arguments, instead received " << numArgs-1 << ")" << endl;
        exit(1);
    }
}

void checkInput(string fileName)
{
    ifstream inFS;
    inFS.open(fileName);
    if(!inFS)
    {
        cout << "One or more of the input files does not exist." << endl;
        exit(1);
    }
    inFS.close();
}

vector<string> readFiles(string fileName)
{
    vector<string> tobeFilled;
    string l;
    ifstream inFS;
    inFS.open(fileName);
    while(getline(inFS, l))
    {
        tobeFilled.push_back(l);
    }
    inFS.close();
    return tobeFilled;
}

void validOutput(string fileName)
{
    string textFile = ".txt"; //to check if the output file is valid
    //if the output file is not in a valid format (***.txt), reports it and closes the program
    if(fileName.size() <= textFile.size() || fileName.compare(fileName.size() - textFile.size(), textFile.size(), textFile) != 0)
    {
        cout << "invalid arguments (the output file given is not in .txt format)" << endl;
        exit(1);
    }
}

void checkThreads(string numThreads)
{
    for(char const &c : numThreads)
    {
        if(isdigit(c) == 0)
        {
            cout << "invalid arguments (number of threads must be a whole number)" << endl;
            exit(1);
        }
    }
    //if they are valid, converts the number of steps and number of threads to integers
    int threadInt = stoi(numThreads);
    //checks to ensure that at least 1 thread is specified, if not, reports and closes the program
    if(threadInt < 1)
    {
        cout << "invalid arguments (number of threads cannot be less than 1)" << endl;
        exit(1);
    }
}

void inputValidity(string keywords, string texttoread, string outputFile, string numThreads)
{
    checkInput(keywords);
    checkInput(texttoread);
    validOutput(outputFile);
    checkThreads(numThreads);
}

int maxThreads(string threads, vector<string> sentences)
{
    int intThreads = stoi(threads);
    if(intThreads > sentences.size())
    {
        intThreads = sentences.size();
    }
    return intThreads;
}

void runProgram(vector<string> words, vector<string> sentences, int numThreads)
{
    thread* myThreads = new thread[numThreads]; //creates the new threads
    for(int t = 0; t < numThreads; ++t)
    {
        myThreads[t] = thread(wordCounter, numThreads, t, words, sentences);
        myThreads[t].join();
    }
    delete[] myThreads;
}

int main(int argc, char** argv)
{
    checkArgs(argc);
    string keywords = argv[1]; //takes in the file to be read
    string texttoread = argv[2];
    string outputFile = argv[3]; //takes in the file to be written to
    string threadInput = argv[4];
    inputValidity(keywords, texttoread, outputFile, threadInput);
    vector<string> words = readFiles(keywords);
    vector<string> sentences = readFiles(texttoread);
    int numThreads = maxThreads(threadInput, sentences);
    runProgram(words, sentences, numThreads);    
}