//
// Created by Frank on 2020-03-31.
//

#ifndef LEXICAL_ANALYSIS_TOKEN_H
#define LEXICAL_ANALYSIS_TOKEN_H


#include <iostream>
#include <string>

using namespace std;
class Token{
public:
    Token(string symbol, string category, int line_number);
    virtual ~Token() {}
    inline string& getSymbol() { return symbol; }
    inline string& getCatergory() { return category; }
    inline string& getToken() { return token; }
    inline string& getType() { return type; }
    inline int getLineNumber() { return line_number; };
    inline int getScope() { return scope; }

    void setType(string type);
    void setScope(int scope);

private:
    string symbol;
    string token;
    string type;
    string category;
    int scope;
    int line_number;


public:
    /* define the DFA move state */
    enum {
        OPERATORS1 = 1,
        OPERATORS2,
        OPERATORS3,
        OPERATORS4,
        COMMENT,
        CHAR,
        SPECIAL_CHAR,
        WAIT_QUOTE,
        SYMBOLS,
        NUMBER,
        IDENTIFIER,
        ACCEPT,
        NOT_ACCEPT
    };
};



#endif //LEXICAL_ANALYSIS_TOKEN_H
