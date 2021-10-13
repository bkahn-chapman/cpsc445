#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <thread>
#include <vector>
using namespace std;

class Program
{
    public:
        vector<int> totals;
        void wordCounter(vector<string> words, vector<string> sentence, int numThreads, int threadNum);
        void checkArgs(int numArgs);
        void checkInput(string fileName);
        vector<string> readFiles(string fileName);
        void validOutput(string fileName);
        void checkThreads(string numThreads);
        void inputValidity(string keywords, string texttoread, string outputFile, string numThreads);
        int maxThreads(string threads, vector<string> sentences);
        void runProgram(vector<string> words, vector<string> sentences, int numThreads, string outputFile);
};

void Program::wordCounter(vector<string> words, vector<string> sentence, int numThreads, int threadNum)
{
    for(int s = 0; s < sentence.size(); ++s)
    {
        if(s % numThreads == threadNum)
        {
            stringstream ss(sentence[s]);
            while(ss)
            {
                string word;
                ss >> word;
                for(int w = 0; w < words.size(); ++w)
                {
                    if(word == words[w])
                    {
                        totals[w]++;
                    }
                }
            }
        }
    }
}


void Program::checkArgs(int numArgs)
{
    if((numArgs-1) != 4)
    {
        cout << "invalid arguemnts (expected 4 arguments, instead received " << numArgs-1 << ")" << endl;
        exit(1);
    }
}

void Program::checkInput(string fileName)
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

vector<string> Program::readFiles(string fileName)
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

void Program::validOutput(string fileName)
{
    string textFile = ".txt"; //to check if the output file is valid
    //if the output file is not in a valid format (***.txt), reports it and closes the program
    if(fileName.size() <= textFile.size() || fileName.compare(fileName.size() - textFile.size(), textFile.size(), textFile) != 0)
    {
        cout << "invalid arguments (the output file given is not in .txt format)" << endl;
        exit(1);
    }
}

void Program::checkThreads(string numThreads)
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

void Program::inputValidity(string keywords, string texttoread, string outputFile, string numThreads)
{
    checkInput(keywords);
    checkInput(texttoread);
    validOutput(outputFile);
    checkThreads(numThreads);
}

int Program::maxThreads(string threads, vector<string> sentences)
{
    int intThreads = stoi(threads);
    if(intThreads > sentences.size())
    {
        intThreads = sentences.size();
    }
    return intThreads;
}

void Program::runProgram(vector<string> words, vector<string> sentences, int numThreads, string outputFile)
{
    for(int w = 0; w < words.size(); ++w)
    {
        totals.push_back(0);
    }
    thread* myThreads = new thread[numThreads]; //creates the new threads
    for(int t = 0; t < numThreads; ++t)
    {
        myThreads[t]=thread(&Program::wordCounter, this, words, sentences, numThreads, t);
        myThreads[t].join();
    }
    delete[] myThreads;
    sort(words.begin(), words.end());
    ofstream outFS;
    outFS.open(outputFile, ios::app);
    for(int i = 0; i < totals.size(); ++i)
    {
        outFS << words[i] << " " << totals[i] << endl;
    }
    outFS.close();
}

int main(int argc, char** argv)
{  
    Program prog;
    prog.checkArgs(argc);
    string keywords = argv[1]; //takes in the file to be read
    string texttoread = argv[2];
    string outputFile = argv[3]; //takes in the file to be written to
    string threadInput = argv[4];
    prog.inputValidity(keywords, texttoread, outputFile, threadInput);
    vector<string> words = prog.readFiles(keywords);
    vector<string> sentences = prog.readFiles(texttoread);
    int numThreads = prog.maxThreads(threadInput, sentences);
    prog.runProgram(words, sentences, numThreads, outputFile);    
}