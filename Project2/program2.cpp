//Name: Nate Bryan
//File Name: program2.cpp
//Date: 27th April, 2024
//Description: Program to read and process ndfas from a given text file
#include <iostream>
#include <fstream>
#include <csignal>
#include <vector>
#include "ndfa,h"

using namespace std;

vector<string> transitions;

void signalHandler(int signal);

bool process(NDFA newNDFA, string& uInput, int layer);

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

    string state = "";
    string stack = "";
    string curState;

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

    ifstream ndfaFile(fileName);

    if(!ndfaFile.is_open())
    {
        cout << "Error: unable to open file.\n";\

        return 1;
    }
    else
    {
        cout << "file opened successfully.\n";
    }

    NDFA newNDFA;

    while(getline(ndfaFile, nextLine))
    {

        nextLine.erase(std::remove_if(nextLine.begin(), nextLine.end(), [](unsigned char c) { return std::isspace(c); }), nextLine.end());

        //cout << "*" << nextLine << "*" << endl;
        
        if(nextLine[0] == '<')
        {
            //nextLine.erase(std::remove_if(nextLine.begin(), nextLine.end(), [](unsigned char c) { return std::isspace(c); }), nextLine.end());
        
            step = nextLine;
        }

        

        //add states to automata
        if(step.find("<states>") != std::string::npos && nextLine[0] != '<')
        {
            newNDFA.addState(nextLine); 
        }

        //add alphabet to automata
        else if(step.find("<inputalphabet>") != std::string::npos && nextLine[0] != '<')
        {
            newNDFA.addSymbol(nextLine[0]);
        }

        //add stack alphabet to automata
        else if(step.find("<stackalphabet>") != std::string::npos && nextLine[0] != '<')
        {
            newNDFA.addStackSymbol(nextLine[0]);
        }

        //add transitions to automata
        else if(step.find("<transitions>") != std::string::npos && nextLine[0] != '<')
        {
            state1 = "";
            state2 = "";
            newT = "";

            for(int i = 0; i < 8; i++)
            {
                if(i < 2)
                {
                    state1 += nextLine[i];
                }
                else if(i == 2)
                {
                    sym = nextLine[i];
                }
                else if(i == 3)
                {
                    stackT = nextLine[i];
                }
                else if( i > 3 && i < 6)
                {
                    state2 += nextLine[i];
                }
                else if( i >=6  && nextLine[i] != ' ')
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
        }

        //sets initial state for automata
        else if(step.find("<initialstate>") != std::string::npos && nextLine[0] != '<')
        {
            newNDFA.addInitialState(nextLine);
        }

        else if(step.find("<stackstart>")!= std::string::npos && nextLine[0] != '<')
        {
            newNDFA.addStackStart(nextLine[0]);
        }

        //sets final states to automata
        else if(step.find("<finalstates>") != std::string::npos && nextLine[0] != '<')
        {
            newNDFA.addFinalStates(nextLine);
        }
    }

    newNDFA.addTransition("*", '*', '*', "*", "*");

    ndfaFile.close();

    newNDFA.displayNDFA();

    
    while(true)
    {
        cout << "Enter a string to process (CTRL^C to end): ";
        getline(cin, uInput);
        cout << endl;

        newNDFA.setCurAsInit();

        curState = newNDFA.getCurState();

        
        state = curState.substr(0, 2);

        if(curState.length() > 2)
        {  
            stack = curState.substr(2, curState.length() - 2);
        }
        else
        {
            stack = "";
        }
        
        

        transitions.clear();

        if(process(newNDFA, uInput, 0))
        {

            if(uInput.length() > 0)
            {
                cout << " (" << state << "," << uInput << "," << stack << ")\n";
            }
            else
            {
                cout << " (" << state << ",*," << stack << ")\n";
            }

            for(int i = transitions.size() - 1; i > -1 ; i--)
            {
                cout << "|-" << transitions.at(i);
            }

            cout << endl << "(Accepted)\n\n";
        }
        else
        {
            cout << "(Rejected)\n\n";
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

bool process(NDFA newNDFA, string& uInput, int layer)
{

    if(!newNDFA.validCheck(uInput[0]) && uInput[0] != '*' && uInput.length() != 0)
    {
        cout << " (Invalid symbol " << uInput[0] << ")";
        return false;
    }

    string state = "";
    string stack = "";
    string curState;

    curState = newNDFA.getCurState();
    state = curState.substr(0, 2);

    if(curState.length() > 2)
    {  
        stack = curState.substr(2, curState.length() - 2);
    }
    else
    {
        stack = "*";
    }

    if(newNDFA.finalCheck() && uInput.length() == 0)
    {
        string transitionStr = "(" + state + ",*," + stack + ")\n";

        if (std::find(transitions.begin(), transitions.end(), transitionStr) == transitions.end()) 
        {
            
            transitions.push_back(transitionStr);
            return true;
        }
    }

    bool accepted = false;
    bool isStack = false;

    curState = newNDFA.getCurState();

    vector<NDFA> ndfaBuffers;
    vector<string> nextStates;

    if(uInput[0] != '\0')
    {
        nextStates = newNDFA.possibleNextStates(state, uInput[0], stack[0]);
    }
    else
    {
        nextStates = newNDFA.possibleNextStates(state, '*', stack[0]);
    }

    for(int i = 0; i < nextStates.size(); i++)
    {
        NDFA s = *new NDFA(newNDFA);
        ndfaBuffers.push_back(s);
    }
    
    for(int i = 0; i < ndfaBuffers.size(); i++)
    {
        string newInput;

        if(uInput.length() > 1 && nextStates.at(i).find('*') == std::string::npos)
        {
            newInput = nextStates.at(i) + uInput.substr(1);
        }
        else 
        {
            newInput = nextStates.at(i);
        }

        string input = newInput.substr(1);

        ndfaBuffers.at(i).transition(newInput[0], stack[0]);

        if(nextStates.at(i) != "*")
        {
            if(process(ndfaBuffers.at(i), input, (layer + 1)) == true)
            {
                accepted = true;
            }
        }
        else
        if(process(ndfaBuffers.at(i), uInput, (layer + 1)) == true)
        {
            accepted = true;
        }
    }

    if(accepted == true)
    {
        if(uInput.length() > 0 && layer != 0)
        {
            transitions.push_back("(" + state + "," + uInput + "," + stack + ")\n");
        }
        else if(layer != 0)
        {
            transitions.push_back("(" + state + ",*," + stack + ")\n");

        }

        return true;
    }

    return false;

}