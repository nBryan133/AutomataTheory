#include <iostream>
#include <tuple>
#include "ndfa,h"

using namespace std;

//implementing member functions
NDFA::NDFA()
{

}

//----------------------------------------------------------------
void NDFA::addState(string s)
{
   states.push_back(s);
}

//----------------------------------------------------------------
void NDFA::addSymbol(char sym)
{
    symbols.push_back(sym);
}

//----------------------------------------------------------------
void NDFA::addStackSymbol(char sym)
{
    stackSym.push_back(sym);
}

//----------------------------------------------------------------
void NDFA::addTransition(string state, char sym, char stackSym, string nextState, string sTop)
{
    tuple<string, char, char> key = make_tuple(state, sym, stackSym);
    vector<pair<string, string>> value = {{nextState, sTop}};

    transitions[key] = value;
}

//----------------------------------------------------------------
void NDFA::addInitialState(string is)
{
    iState = is;
}

//----------------------------------------------------------------
void NDFA::addFinalStates(string fs)
{
    fStates.push_back(fs);
}

//----------------------------------------------------------------
void NDFA::addStackStart(char sym)
{
    iStack = sym;
}

//----------------------------------------------------------------
void NDFA::displayNDFA()
{
    cout << "----------D F A----------" << endl;
    cout << "<states>\n";

    for(int i = 0; i < states.size(); i++)
    {
        cout << states[i] << endl;
    }

    cout << endl;
    cout << "<input alphabet>\n";

    for(int i = 0; i < symbols.size(); i++)
    {
        cout << symbols[i] << endl;
    }

    cout << endl;
    cout << "<Transitions>\n";

    for(auto it = transitions.begin(); it != transitions.end(); it++)
    {
        const auto& stateSym = it->first;
        const auto& newState = it->second;

        cout << get<0>(stateSym) << " " << get<1>(stateSym) << " " << get<2>(stateSym);

        for (const auto& pair : newState)
        {
            cout << pair.first << " " << pair.second << endl;
        }
    }

    cout << "<initial state>\n" << iState << endl;

    cout << "<Stack Start>\n";

    for(int i = 0; i < stackSym.size(); i++)
    {
        cout << stackSym[i] << endl;
    }

    cout << "<final states>\n";

    for(int i = 0; i < fStates.size(); i++)
    {
        cout << fStates[i] << endl;
    }

    cout << endl;
    cout << "-------------------------\n";
}

//----------------------------------------------------------------
void NDFA::setCurAsInit()
{
    curState = iState;
}

//----------------------------------------------------------------
void NDFA::transition(char sym)
{
    tuple<string, char, char> nextTransition = make_tuple(curState, sym, stackSym);   //makes tuple to look for transition

    auto it = transitions.find(nextTransition);                     //looks for transition with given symbol from current state

    if(it != transitions.end())
    {
        curState = it->second;
    }
}

//----------------------------------------------------------------
string NDFA::getCurState()
{
    return curState;
}

//----------------------------------------------------------------
bool NDFA::validCheck(char sym)
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
bool NDFA::finalCheck()
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