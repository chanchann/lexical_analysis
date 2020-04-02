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
#include <unistd.h>
#include <sys/stat.h>

#include "token.h"
#include "state.h"
using namespace std;

class Lexier{
public:
    Lexier(string input_path);
    ~Lexier();
    vector<Token*> getTokenList();
    bool startParseTokens();

private:
    ifstream& getInput();
    ofstream& getOfstream();
    int create_dir_not_exist(string path);
    void handle(const std::string& src);
    void handleState(const char c);
    void initToken(const char c);

private:
    inline bool isAlpha(const char& c) {
        if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
            return true;
        return false;
    }
    inline bool isDigital(const char& c) {
        if (c >= '0' && c <= '9')
            return true;
        return false;
    }

private:
    string input_path;
    vector<Token*> token_list;
    Dfstate state;
};

#endif //LEXICAL_ANALYSIS_LEXIER_H
