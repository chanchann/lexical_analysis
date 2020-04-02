//
// Created by Frank on 2020-03-31.
//

#ifndef LEXICAL_ANALYSIS_TOKEN_H
#define LEXICAL_ANALYSIS_TOKEN_H


#include <iostream>
#include <string>
#include "state.h"
using namespace std;
class Token{
public:
    Token();
    Token(string symbol, string category, int line_number);
    ~Token() {}
    inline string getSymbol() const { return symbol; }
    inline string getCatergory() const { return category; }
    inline string getToken() const { return token; }
    inline string getType() const { return type; }
    inline int getLineNumber() const { return line_number; };
    inline int getScope() const { return scope; }

    void setType(TokenType& type);
    void setScope(int scope);

private:
    string symbol;
    string token;
    TokenType type;
    string category;
    int scope;
    int line_number;

};



#endif //LEXICAL_ANALYSIS_TOKEN_H
