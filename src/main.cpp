//
// Created by Frank on 2020-03-31.
//

#include <iostream>
#include "lexier.h"
#include "token.h"
using namespace std;

int main(int argc, char** argv){
    Lexier* lexier;

    /* Start Lexial Analysis */
    cout << " ---- Start Lexial Analysis ----" << endl;
//    lexier = new Lexier(argv[1]);
    lexier = new Lexier("../test/test_data/test.c");
    if (lexier -> startParseTokens()){
        cout << "--Lexial Analysis success!" << endl;
    }
    else{
        cout << "Lexier Analysis error." << endl;
        exit(1);
    }

    delete lexier;
}