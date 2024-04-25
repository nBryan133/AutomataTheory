#include "dfa,h"
#include <iostream>

using namespace std;

//implementing member functions
DFA::DFA()
{

}

//----------------------------------------------------------------
void DFA::addState(string s)
{
   states.push_back(s);
}

//----------------------------------------------------------------
void DFA::addSymbol(char sym)
{
    symbols.push_back(sym);
}

//----------------------------------------------------------------
void DFA::addTransition(string state, char symbol, string nextState)
{
    transitions.insert(make_pair(make_pair(state, symbol), nextState));
}

//----------------------------------------------------------------
void DFA::addInitialState(string is)
{
    iState = is;
}

//----------------------------------------------------------------
void DFA::addFinalStates(string fs)
{
    fStates.push_back(fs);
}

//----------------------------------------------------------------
void DFA::displayDFA()
{
    cout << "----------D F A----------" << endl;
    cout << "<states>\n";

    for(int i = 0; i < states.size(); i++)
    {
        cout << states[i] << " ";
    }

    cout << endl;
    cout << "<alphabet>\n";

    for(int i = 0; i < symbols.size(); i++)
    {
        cout << symbols[i] << " ";
    }

    cout << endl;
    cout << "<Transitions>\n";

    for(auto it = transitions.begin(); it != transitions.end(); it++)
    {
        const auto& stateSym = it->first;
        const auto& newState = it->second;

        cout << "(" << stateSym.first << ", " << stateSym.second << ")->" << newState << endl;
    }

    cout << "<initial state>\n" << iState << endl;

    cout << "<final states>\n";

    for(int i = 0; i < fStates.size(); i++)
    {
        cout << fStates[i] << " ";
    }

    cout << endl;
    cout << "-------------------------\n";
}

//----------------------------------------------------------------
void DFA::setCurAsInit()
{
    curState = iState;
}

//----------------------------------------------------------------
void DFA::transition(char sym)
{
    pair<string, char> nextTransition = make_pair(curState, sym);   //makes pair to look for transition

    auto it = transitions.find(nextTransition);                     //looks for transition with given symbol from current state

    if(it != transitions.end())
    {
        curState = it->second;
    }
}

//----------------------------------------------------------------
string DFA::getCurState()
{
    return curState;
}

//----------------------------------------------------------------
bool DFA::validCheck(char sym)
{
    bool isValid = false;

    for(int i = 0; i < symbols.size(); i++)
    {
        if(sym == symbols[i])
        {
            isValid = true;
        }
    }

    return isValid;
}

//----------------------------------------------------------------
bool DFA::finalCheck()
{
    bool isFinal = false;

    for(int i = 0; i < fStates.size(); i++)
    {
        if(curState == fStates[i])
        {
            isFinal = true;
        }
    }

    return isFinal;
}

//----------------------------------------------------------------