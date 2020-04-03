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

using namespace std;

class Lexier{
public:
    Lexier(string input_path);
    ~Lexier();
    vector<Token*>& getTokenList();
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
    inline bool isBlank(int ch) {
        return ch == ' ' || ch == '\t' || ch == '\n';
    }

private:
    enum Dfstate {
        Initial,

//        If, Id_if1, Id_if2, Else, Id_else1, Id_else2, Id_else3, Id_else4, Int,
        Id_int1, Id_int2, Id_int3, Id, GT, GE,

        Assignment,

        Plus, Minus, Star, Slash,

        SemiColon,
        LeftParen,
        RightParen,

        IntLiteral
    };

private:
    string input_path;
    Dfstate state;

    //下面几个变量是在解析过程中用到的临时变量,如果要优化的话，可以塞到方法里隐藏起来
    string tokenText; // 用于临时保存获取的字符序列
    vector<Token*> token_list; // 保存解析出来的Token
    Token* token; //当前正在解析的Token
    int line_number;
};

#endif //LEXICAL_ANALYSIS_LEXIER_H
