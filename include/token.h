//
// Created by Frank on 2020-03-31.
//

#ifndef LEXICAL_ANALYSIS_TOKEN_H
#define LEXICAL_ANALYSIS_TOKEN_H


#include <iostream>
#include <string>
#include <unordered_map>
#include "tokenType.h"

using namespace std;
class Token{
    friend class Lexier;
public:
    Token();
    ~Token() {}
    inline string getText() const { return text; }
    inline TokenType getType() const { return type; }
    inline int getLineNumber() const { return line_number; }
    inline string getStateStr(const TokenType& key) const { return tagLexemeMap_.at(key);}

    inline void setType(const TokenType& type) { this->type = type; };
    inline void setText(const string& text) { this->text = text; }
    inline void setLineNumber(const int lineNumber) { this->line_number = lineNumber; }

private:
    string text;
    TokenType type;
    int line_number;
    static const unordered_map<enum TokenType, string> kwTypeMap_;
    static const unordered_map<TokenType, string> tagLexemeMap_;
};



#endif //LEXICAL_ANALYSIS_TOKEN_H
