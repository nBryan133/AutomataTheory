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
    string newT;
    string uInput;

    char sym;
    char stackT;

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

    NDFA newNDFA;

    while(getline(dfaFile, nextLine))
    {

        //nextLine.erase(std::remove_if(nextLine.begin(), nextLine.end(), [](unsigned char c) { return std::isspace(c); }), nextLine.end());
        
        if(nextLine[0] == '<')
        {
            nextLine.erase(std::remove_if(nextLine.begin(), nextLine.end(), [](unsigned char c) { return std::isspace(c); }), nextLine.end());
        
            step = nextLine;
        }

        

        //add states to automata
        if(step == "<states>" && nextLine[0] != '<')
        {
            newNDFA.addState(nextLine); 
        }

        //add alphabet to automata
        else if(step == "<inputalphabet>" && nextLine[0] != '<')
        {
            newNDFA.addSymbol(nextLine[0]);
        }

        //add stack alphabet to automata
        else if(step == "<stackalphabet>" && nextLine[0] != '<')
        {
            newNDFA.addStackSymbol(nextLine[0]);
        }

        //add transitions to automata
        else if(step == "<transitions>" && nextLine[0] != '<')
        {
            for(int i = 0; i < 12; i++)
            {
                if(i < 2)
                {
                    state1 += nextLine[i];
                }
                else if(i == 3)
                {
                    sym = nextLine[i];
                }
                else if(i == 5)
                {
                    stackT = nextLine[i];
                }
                else if( i > 6 && i < 9)
                {
                    state2 += nextLine[i];
                }
                else if( i > 9)
                {
                    newT += nextLine[i];
                }
            }

            /*cout << "state1: " << state1 << 
            "\n sym: " << sym << 
            "\n stackT: " << stackT << 
            "\n state2: " << state2 << 
            "\n newT: " << newT << endl << endl;*/

            newNDFA.addTransition(state1, sym, stackT, state2, newT);
            state1 = "";
            state2 = "";
            newT = "";
        }

        //sets initial state for automata
        else if(step == "<initialstate>" && nextLine[0] != '<')
        {
            newNDFA.addInitialState(nextLine);
        }

        else if(step =="<stackstart>" && nextLine[0] != '<')
        {
            newNDFA.addStackStart(nextLine[0]);
        }

        //sets final states to automata
        else if(step == "<finalstates>" && nextLine[0] != '<')
        {
            newNDFA.addFinalStates(nextLine);
        }
    }

    dfaFile.close();

    newNDFA.displayNDFA();

    //loop for the user to input strings to be read by the automata
    while(true)
    {

        string state = "";
        string stack = "";
        string curState;

        bool isStack = false;

        newNDFA.setCurAsInit();
        invalid = false;
         
        cout << "Enter a string to process (CTRL^C to end): ";
        getline(cin, uInput);
        cout << endl;

        isStack = false;
        state = "";
        stack = "";

        curState = newNDFA.getCurState();

        cout << "\n\ncurState: " << curState;

        for(int i = 0; i < curState.length(); i++)
        {
            if(curState[i] == ' ')
            {
                isStack = true;
            }

            if(isStack == false)
            {
                state += curState[i];
            }
            else
            {
                stack += curState[i];
            }
        }
        
        cout << " (" << state << "," << uInput << "," << stack << ")\n";

        for(int i = 0; i < uInput.length(); i++)
        {
            if(!newNDFA.validCheck(uInput[i]))
            {
                cout << " (Invalid symbol " << uInput[i] << ")";
                invalid = true;
                break;
            }

            newNDFA.transition(uInput[i]);

            isStack = false;
            state = "";
            stack = "";
            curState = newNDFA.getCurState();

            for(int i = 0; i < curState.length(); i++)
            {
                if(curState[i] == ' ')
                {
                    isStack = true;
                }

                if(isStack == false)
                {
                    state += curState[i];
                }
                else
                {
                    stack += curState[i];
                }
            }

            cout << "|- " << "(" << state << ",";
            
            for(int n = i + 1; n <= uInput.length();n++)
            {
                if(i + 1 < uInput.length())
                {
                    cout << uInput[n];
                }
                else
                {
                    cout << "*";
                }
            }
            
            cout << "," << stack << ")\n";

        }


        if(newNDFA.finalCheck() && invalid == false)
        {
            cout << " : Accepted\n\n";
        }
        else
        {
            cout << " : Rejected\n\n";
        }
    }

    return 0;
}

//triggers when ctrl^c is used and closes the program
void signalHandler(int signal)
{
    cout << "\nExiting Program...\n";

    exit(signal);
}