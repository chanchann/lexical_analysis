//
// Created by Frank on 2020-04-02.
//

#include "../include/lexier.h"
#include "../include/token.h"
using namespace std;

int main(){
    Lexier* lexier = new Lexier("../test/test_data/keywords.c");
    lexier->test_startParseTokens();
    return 0;
}