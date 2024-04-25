//Name: Nate Bryan
//File Name: program2.cpp
//Date: 5th March, 2024
//Description:
#include <iostream>
#include <fstream>
#include <csignal>
#include "ndfa,h"

using namespace std;

void signalHandler(int signal);

//main function
int main(int argc, char *argv[])
{
    string fileName;
    string nextLine;
    string step;
    string state1;
    string state2;
    string uInput;

    char sym;

    bool invalid = false;

    //used to check if user used ctrl^C
    signal(SIGINT, signalHandler);

    if(argc < 2)
    {
        cout << "No file name provided.\n";
        return 1;
    }
    else
    {
        fileName = argv[1];
    }

    ifstream dfaFile(fileName);

    if(!dfaFile.is_open())
    {
        cout << "Error: unable to open file.\n";\

        return 1;
    }
    else
    {
        cout << "file opened successfully.\n";
    }

    DFA newDFA;

    while(getline(dfaFile, nextLine))
    {
        if(nextLine[0] == '<')
        {
            step = nextLine;
        }

        //add states to automata
        if(step == "<states>" && nextLine[0] != '<')
        {
            newDFA.addState(nextLine);
        }

        //add alphabet to automata
        else if(step == "<alphabet>" && nextLine[0] != '<')
        {
            newDFA.addSymbol(nextLine[0]);
        }

        //add transitions to automata
        else if(step == "<transitions>" && nextLine[0] != '<')
        {
            for(int i = 0; i < 7; i++)
            {
                if(i < 2)
                {
                    state1 += nextLine[i];
                }
                else if(i == 3)
                {
                    sym = nextLine[i];
                }
                else if( i > 4)
                {
                    state2 += nextLine[i];
                }
            }

            newDFA.addTransition(state1, sym, state2);
            state1 = "";
            state2 = "";
        }

        //sets initial state for automata
        else if(step == "<initial state>" && nextLine[0] != '<')
        {
            newDFA.addInitialState(nextLine);
        }

        //sets final states to automata
        else if(step == "<final states>" && nextLine[0] != '<')
        {
            newDFA.addFinalStates(nextLine);
        }
    }

    dfaFile.close();

    newDFA.displayDFA();

    //loop for the user to input strings to be read by the automata
    /*while(true)
    {
        newDFA.setCurAsInit();
        invalid = false;
         
        cout << "Enter a string to process (CTRL^C to end): ";
        getline(cin, uInput);
        cout << endl;

        cout << "[" << newDFA.getCurState() << "]";

        for(int i = 0; i < uInput.size(); i++)
        {
            if(!newDFA.validCheck(uInput[i]))
            {
                cout << " (Invalid symbol " << uInput[i] << ")";
                invalid = true;
                break;
            }

            cout << "-" << uInput[i] <<"->";

            newDFA.transition(uInput[i]);

            cout << "[" << newDFA.getCurState() << "]";

        }

        if(newDFA.finalCheck() && invalid == false)
        {
            cout << " : Accepted\n\n";
        }
        else
        {
            cout << " : Rejected\n\n";
        }
    }*/

    return 0;
}

//triggers when ctrl^c is used and closes the program
void signalHandler(int signal)
{
    cout << "\nExiting Program...\n";

    exit(signal);
}