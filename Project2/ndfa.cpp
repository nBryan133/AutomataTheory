#include <iostream>
#include <tuple>
#include <vector>
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
    cout << "----------N D F A----------" << endl;
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

    cout << "<Transitions>\n" << flush;

    // Print transitions
    for (auto it = transitions.begin(); it != transitions.end(); it++) {
        const auto& stateSym = it->first;
        const auto& newState = it->second;

        cout << "(" << get<0>(stateSym) << "," << get<1>(stateSym) << "," << get<2>(stateSym) << ") -> ";

        // Print each transition in newState vector
        for (const auto& pair : newState) {
            cout << "(" << pair.first << "," << pair.second << ")" << endl;;
        }

    }

    cout << "<Initial State>\n" << iState << endl;

    cout << "<Stack Start>\n" << iStack << endl;

    // Print final states
    cout << "<Final States>\n";
    for (int i = 0; i < fStates.size(); i++) {
        cout << fStates[i] << endl;
    }

    cout << "---------------------------\n";
}



//----------------------------------------------------------------
void NDFA::setCurAsInit()
{
    string s(1, iStack);
    curState = "";
    curState = (iState + s);
}

//----------------------------------------------------------------
void NDFA::transition(char sym, char stackT)
{

    string state = "";
    string stack = "";

    bool accepted = false;
    bool isStack = false;

    state = curState.substr(0, 2);
    stack = curState.substr(2, curState.length() - 2);

    tuple<string, char, char> nextTransition = make_tuple(state, sym, stackT);   //makes tuple to look for transition

    auto it = transitions.find(nextTransition);

    int i = 0;

    string newState;

    if (it != transitions.end())
    {
        

        for (const auto &pair : it->second)
        {
            newState = "";

            /*cout << "sym: *" << sym << "*" << endl;
            cout << "stack: *" << stack << "*" << endl;
            cout << "stackT: *" << stackT << "*" << endl;

            cout << "pairf: *" << pair.first << "*" << endl;
            cout << "pairs: *" << pair.second << "*" << endl;
            if(stack.length() > 0)
            {
                cout << "pairs+stack: *" << pair.second + stack.substr(1) << "*" << endl;
            }*/

            if(newState.length() == 0)
            {
                newState += pair.first;
            }

            if (!(pair.second.find("*") != std::string::npos))
            {
                if(stack.length() > 0 && pair.second != "" && stack.substr(1) != "")
                {
                    newState += pair.second;
                    newState += stack.substr(1);
                }
                else if(pair.second != "")
                {
                    newState += pair.second;
                }
                
            }
            else 
            {
                if(stack.length() > 0)
                {
                    newState += stack.substr(1);
                }
                    
            }

            i++;

        }

        curState = "";

        for(int i = 0; i < newState.length(); i++)
        {
            if(newState[i] != '\0')
            {
                curState += newState[i];
            }
        }

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

    for(int i = 0; i < symbols.size(); i++)
    {
        if(sym == symbols[i])
        {
            return true;
        }

        if(sym == '*')
        {
            return true;
        }
    }

    return false;
}

//----------------------------------------------------------------
bool NDFA::finalCheck()
{
    bool isFinal = false;

    string state = curState.substr(0, 2);

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

vector<string> NDFA::possibleNextStates(string state, char sym, char stackT)
{
    string nextStates;

    vector<string> possibleNextStates;

    tuple<string, char, char> nextTransition = make_tuple(state, sym, stackT);   //makes tuple to look for transition
    tuple<string, char, char> nextTwA = make_tuple(state, '*', stackT);   //makes tuple to look for transition for empty string top

    auto it = transitions.find(nextTransition);

    if (it != transitions.end())
    {
        
        possibleNextStates.push_back(string(1, sym));
        
    }

    it = transitions.find(nextTwA);

    if (it != transitions.end())
    {
        
        possibleNextStates.push_back("*");
        
    }

    return possibleNextStates;
}