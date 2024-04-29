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
    vector< pair<string, string> > value = {{nextState, sTop}};

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
void NDFA::displayNDFA() {
    cout << "----------D F A----------" << endl;
    cout << "<States>\n";

    // Print states
    for (int i = 0; i < states.size(); i++) {
        cout << states[i] << endl;
    }

    cout << "<Input Alphabet>\n";

    // Print input alphabet
    for (int i = 0; i < symbols.size(); i++) {
        cout << symbols[i] << endl;
    }

    cout << "<Stack Alphabet>\n";

    // Print stack alphabet
    for (int i = 0; i < stackSym.size(); i++) {
        cout << stackSym[i] << endl;
    }

    cout << "<Transitions>\n";

    // Print transitions
    for (auto it = transitions.begin(); it != transitions.end(); it++) {
        const auto& stateSym = it->first;
        const auto& newState = it->second;

        cout << " (" << get<0>(stateSym) << "," << get<1>(stateSym) << "," << get<2>(stateSym) << ") -> ";

        // Print each transition in newState vector
        for (const auto& pair : newState) {
            cout << "(" << pair.first << "," << pair.second;
        }

        cout << ")" << endl;
    }

    cout << "<Initial State>\n" << iState << endl;
    cout << "<Stack Start>\n" << iStack << endl;

    // Print final states
    cout << "<Final States>\n";
    for (int i = 0; i < fStates.size(); i++) {
        cout << fStates[i] << endl;
    }

    cout << "-------------------------\n";
}



//----------------------------------------------------------------
void NDFA::setCurAsInit()
{
    curState = (iState + " " + iStack);
}

//----------------------------------------------------------------
void NDFA::transition(char sym)
{

    string state = "";
    string stack = "";

    bool isStack = false;
    
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

    tuple<string, char, char> nextTransition = make_tuple(state, sym, stack[0]);   //makes tuple to look for transition

    auto it = transitions.find(nextTransition);

    curState = "";

    int i = 0;

    if (it != transitions.end())
    {
        for (const auto &pair : it->second)
        {
            curState += pair.first;
            curState += pair.second;
            i++;
        }
    }

    if(i == 0)
    {
        i = 0;
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

    string state = "";

    for(int i = 0; i < 2; i++)
    {
        state += curState[i];
    }

    for(int i = 0; i < fStates.size(); i++)
    {
        if(state == fStates[i])
        {
            isFinal = true;
        }
    }

    return isFinal;
}

//----------------------------------------------------------------