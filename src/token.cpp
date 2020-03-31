//
// Created by Frank on 2020-03-31.
//

#include "token.h"

Token::Token(string symbol, string category, int line_number)
    :symbol(symbol), category(category),line_number(line_number){
    if(category.compare("Identifier") == 0) {
        token = "id";
    }else{
        token = category;
    }
}

void Token::setType(string type){
    this->type = type;
}

void Token::setScope(int scope){
    this->scope = scope;
}
