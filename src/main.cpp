//
// Created by Frank on 2020-03-31.
//

#include <iostream>
#include "lexier.hpp"
#include "token.hpp"
using namespace std;

void test1(){
    Token* tk = new Token();
    tk->setLineNumber(1);
    tk->setType(TokenType::Slash);
    cout << tk->getStateStr(TokenType::Slash) << endl;
    cout << tk->getLineNumber() << endl;
    delete tk;
}

int main(int argc, char** argv){

//    test1();
    Lexier* lexier;

    /* Start Lexial Analysis */
    cout << " ---- Start Lexial Analysis ----" << endl;
//    lexier = new Lexier(argv[1]);
//    lexier = new Lexier("../test/test_data/test.c");
    lexier = new Lexier("../test/test_data/keywords.c");
    if (lexier -> startParseTokens()){
        cout << "--Lexial Analysis success!" << endl;
    }
    else{
        cout << "Lexier Analysis error." << endl;
        exit(1);
    }
//    vector<Token*> res = lexier->getTokenList();
//    for (auto iter = res.begin(); iter != res.end(); iter++)
////        cout << tagLexemeMap_[iter->getType()] << ": " << iter->getText() << std::endl;
//        cout  << (*iter)->getStateStr((*iter)->getType()) << ": " << (*iter)->getText() << std::endl;

    delete lexier;
}