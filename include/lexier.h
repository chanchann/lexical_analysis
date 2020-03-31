//
// Created by Frank on 2020-03-31.
//

#ifndef LEXICAL_ANALYSIS_LEXIER_H
#define LEXICAL_ANALYSIS_LEXIER_H

#include <iostream>
#include <vector>
#include <string>
#include <cstring>
#include <cstdlib>
#include <fstream>
#include <iomanip>

#include "token.h"

using namespace std;

class Lexier{
public:
    Lexier(string input_path);
    ~Lexier();
    vector<Token*> getTokenList();
    bool startParseTokens();

private:
    bool enterDfa(string token, string& tokenCategory);
    void startState(int& state, char next_char, string& tokenCategory);
    void move(int& state, char next_char, string& tokenCategory);
    ifstream& getInput();
    ofstream& getOfstream();

private:
    string input_path;
    vector<Token*> token_list;
    int keyword_move;
    int state;

};

#endif //LEXICAL_ANALYSIS_LEXIER_H
