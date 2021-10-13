/*
* Name: Benjamin Kahn
* ID: 2316793
* Class: CPSC-445-01
* Assignment: Assignment 2
* About: Finds how many times each of a list of given words occurs in a given set of text with a given number of threads. Outputs to a given file.
*/

#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <thread>
#include <vector>
using namespace std;

// Defines all of the public variables to be used throughout the program (especially the totals vector).
class Program
{
    public:
        vector<int> totals; //tracks the total number of times each word is found accross all threads/lines
        void wordCounter(vector<string> words, vector<string> sentence, int numThreads, int threadNum); //counts each word on a per-thread basis
        void checkArgs(int numArgs); //checks to see if the proper number of arguments is given
        void checkInput(string fileName); //checks if the input file given exists
        vector<string> readFiles(string fileName); //reads through the given files to create string vectors
        void validOutput(string fileName); //checks if the given output file is valid
        void checkThreads(string numThreads); //checks if the number of threads given is valid
        void inputValidity(string keywords, string texttoread, string outputFile, string numThreads); //combines all checks into one function for main
        int maxThreads(string threads, vector<string> sentences); //ensures there aren't more threads than lines in the input file
        void runProgram(vector<string> words, vector<string> sentences, int numThreads, string outputFile); //runs the program and outputs results
};

//counts each word on a per-thread basis
void Program::wordCounter(vector<string> words, vector<string> sentence, int numThreads, int threadNum)
{
    for(int s = 0; s < sentence.size(); ++s) //checks every sentence in the given input file
    {
        if(s % numThreads == threadNum) //cyclical distribution of threads
        {
            stringstream ss(sentence[s]);
            while(ss) //while there are words in the sentence
            {
                string word;
                ss >> word;
                for(int w = 0; w < words.size(); ++w) //checks each given keyword
                {
                    if(word == words[w]) 
                    {
                        totals[w]++; //adds to that word's vector position if it is found
                    }
                }
            }
        }
    }
}

//checks to see if the proper number of arguments is given
void Program::checkArgs(int numArgs)
{
    if((numArgs-1) != 4) //makes sure there are exactly four arguments given
    {
        cout << "invalid arguemnts (expected 4 arguments, instead received " << numArgs-1 << ")" << endl;
        exit(1);
    }
}

//checks if the input file given exists
void Program::checkInput(string fileName)
{
    ifstream inFS;
    inFS.open(fileName);
    if(!inFS) //checks if the given file exists
    {
        cout << "One or more of the input files does not exist." << endl;
        exit(1);
    }
    inFS.close();
}

//reads through the given files to create string vectors
vector<string> Program::readFiles(string fileName)
{
    vector<string> tobeFilled; //creates the vector that will be used in later steps
    string l;
    ifstream inFS;
    inFS.open(fileName);
    while(getline(inFS, l)) //iterates through every line in the given file
    {
        l.erase(remove_if(l.begin(), l.end(), ::ispunct), l.end()); //removes punctuation
        tobeFilled.push_back(l); //pushes each line to the end of the vector
    }
    inFS.close();
    return tobeFilled; //the string vector
}

//checks if the given output file is valid
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

//checks if the number of threads given is valid
void Program::checkThreads(string numThreads)
{
    //makes sure every thread is a whole number
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

//combines all checks into one function for main
void Program::inputValidity(string keywords, string texttoread, string outputFile, string numThreads)
{
    checkInput(keywords); //checks if the keyword file is valid
    checkInput(texttoread); //checks if the text to read file is valid
    validOutput(outputFile); //checks if the given output file is valid
    checkThreads(numThreads); //checks if the given number of threads is valid
}

//ensures there aren't more threads than lines in the input file
int Program::maxThreads(string threads, vector<string> sentences)
{
    int intThreads = stoi(threads);
    //prevents there from being more threads than lines in the file as it would leave unused threads
    if(intThreads > sentences.size())
    {
        intThreads = sentences.size();
    }
    return intThreads;
}

//runs the program and outputs results
void Program::runProgram(vector<string> words, vector<string> sentences, int numThreads, string outputFile)
{
    //generates the initial values for the vector of word totals
    for(int w = 0; w < words.size(); ++w)
    {
        totals.push_back(0);
    }
    sort(words.begin(), words.end()); //sorts the words alphabetically
    thread* myThreads = new thread[numThreads]; //creates the new threads
    //sends each of the threads to count the words in its rows (cyclical distribution)
    for(int t = 0; t < numThreads; ++t)
    {
        myThreads[t]=thread(&Program::wordCounter, this, words, sentences, numThreads, t);
        myThreads[t].join();
    }
    delete[] myThreads;
    ofstream outFS;
    outFS.open(outputFile, ios::app);
    //outputs the results to the given file
    for(int i = 0; i < totals.size(); ++i)
    {
        outFS << words[i] << " " << totals[i] << endl;
    }
    outFS.close();
}

int main(int argc, char** argv)
{  
    Program prog; //new instance of the program to have global access to the totals vector
    prog.checkArgs(argc); //checks if the correct number of argumebts is given
    string keywords = argv[1]; //takes in the file of keywords
    string texttoread = argv[2]; //takes in the file of text to be read through
    string outputFile = argv[3]; //takes in the file to be written to
    string threadInput = argv[4]; //takes in the desired number of threads
    prog.inputValidity(keywords, texttoread, outputFile, threadInput); //checks if all given arguments are valid
    vector<string> words = prog.readFiles(keywords); //generates the list of keywords to look for
    vector<string> sentences = prog.readFiles(texttoread); //generates the list of lines from the given file to be looked through
    int numThreads = prog.maxThreads(threadInput, sentences); //corrects for if there are more threads than lines
    prog.runProgram(words, sentences, numThreads, outputFile); //runs the program now that everything is confirmed to be correct
}