#include <iostream>
#include <string>
#include <tuple>
#include <unordered_map>
#include <vector>
#include <fstream>
#include <sstream>

using namespace std;

char toChar(int num);
int charToInt(char sign);
void checkResult(vector<bool> &results);

vector<char> el(const vector<tuple<char, char>>& endStates, char i);
void parseInput(ifstream& file, vector<char>& alphabet, vector<char>& states,
                char& start_state, vector<char>& final_states, unordered_map<char,
                vector<tuple<char, char>>>& transitions);


class NFA
{
public:
    NFA(vector<char> abeceda_,
        vector<char> states_,
        char start_state_,
        vector<char> final_states_,
        unordered_map<char, vector<tuple<char, char>>> transitions_)
    {
        abeceda = abeceda_;
        states = states_;
        start_state = start_state_;
        final_states = final_states_;
        transitions = transitions_;
    }

    void isAccept(char firstState, string row, vector<bool> &results)
    {
        char op = row[0];
        vector<char> endStates = el(transitions[op], firstState);
        if (row.length() == 1)
        {
            checksuccess(endStates, results);
            return;
        }
        row = row.substr(1, row.length() - 1);
        if (!endStates.empty()) {
            for (int i = 0; i < endStates.size(); i++) {
                char state = endStates[i];
                isAccept(state, row, results);
            }
        }

    }

    void checksuccess(vector<char> endStates, vector<bool> &results)
    {
        for (int i = 0; i < endStates.size(); i++) {
            for (int j = 0; j < final_states.size(); j++) {
                if (endStates[i] == final_states[j])
                    results.push_back(1);
                else
                    results.push_back(0);
            }
        }
    }

private:
    vector<char> abeceda;
    vector<char> states;
    char start_state;
    vector<char> final_states;
    unordered_map<char, vector<tuple<char, char>>> transitions;
};


int main()
{
    string input;
    vector<bool> results;
    ifstream file("C:\\Users\\HP\\CLionProjects\\3rdcourse\\SP_lab1_c++\\input.txt");
    if (!file.is_open())
    {
        cout << "Opening error." << endl;
        return 1;
    }

    vector<char> abeceda;
    vector<char> states;
    char start_state;
    vector<char> final_states;
    unordered_map<char, vector<tuple<char, char>>> transitions;

    parseInput(file, abeceda, states, start_state, final_states, transitions);

    NFA automaton(abeceda, states, start_state, final_states, transitions);

    cout << "Enter a row to check: ";
    cin >> input;

    automaton.isAccept(start_state, input, results);
    checkResult(results);
    //automaton.isAccept(start_state, "eac", results);
    //checkResult(results);

    return 0;
}

vector<char> el(const vector<tuple<char, char>>& endStates, char i)
{
    vector<char> res;
    for (int j = 0; j < endStates.size(); j++) {
        const tuple<char, char>& item = endStates[j];
        if (get<0>(item) == i) {
            res.push_back(get<1>(item));
        }
    }
    return res;
}

char toChar(int num) {
    if (num >= 1 && num <= 26) {
        return static_cast<char>('a' + num - 1);
    } else {
        return '/';
    }
}
int charToInt(char sign) {
    return sign - '0';
}

void checkResult(vector<bool> &results)
{
    if (results.empty())
    {
        cout << "This string is rejected." << endl;
        results.clear();
        return;
    }
    for (int i = 0; i < results.size(); i++){
        bool res = results[i];
        if (res)
        {
            cout << "This string is accessible." << endl;
            results.clear();
            return;
        }}
    results.clear();
    cout << "This string is rejected." << endl;
}
void parseInput(ifstream& file, vector<char>& alphabet, vector<char>& states, char& start_state, vector<char>& final_states, unordered_map<char, vector<tuple<char, char>>>& transitions) {
    string row;
    int i = 0;

    while (getline(file, row)) {
        if (i == 3) {
            istringstream iss(row);
            char number;
            iss >> number;
            while (iss >> number) {
                final_states.push_back(number);
            }
        }
        if (i >= 4) {
            istringstream iss(row);
            char elem, state1, state2;
            iss >> state1;
            iss >> elem;
            iss >> state2;
            transitions[elem].push_back(make_tuple(state1, state2));
        }
        for (int j = 0; j < row.size(); j++) {
            char el = row[j];
            if (el != ' ') {
                if (i == 0) {
                    for (int k = 1; k <= charToInt(el); ++k) {
                        alphabet.push_back(toChar(k));
                    }
                }
                if (i == 1) {
                    for (int k = 1; k <= charToInt(el); ++k) {
                        states.push_back(k);
                    }
                }
                if (i == 2) {
                    start_state = el;
                }
            }
        }
        i++;
    }
}

