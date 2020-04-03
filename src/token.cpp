//
// Created by Frank on 2020-04-03.
//

#include "token.h"

const unordered_map<enum TokenType, string> kwTypeMap_ {
        {TokenType::If, "IF"},
        {TokenType::Else, "ELSE"},
        {TokenType::Int, "INT"},
};

const unordered_map<TokenType, string> tagLexemeMap_{
        {TokenType::Plus, "plus : +"},   // +
        {TokenType::Minus, "minus : -"}, // -
        {TokenType::Star, "star : *"},  // *
        {TokenType::Slash, "slash : /"}, //
        {TokenType::GE, "GE : >="},     // >=
        {TokenType::GT, "GT : >"},     //
        {TokenType::EQ, "EQ : =="},    // ==
        {TokenType::LE, "LE : <="},     // <=
        {TokenType::LT, "LT : <"},     // <
        {TokenType::SemiColon, "semicolon : ;"}, // ;
        {TokenType::LeftParen, "leftparen : ("}, // (
        {TokenType::RightParen, "rightparen : )"},// )
        {TokenType::Assignment, "assignment : ="},// =
        // 关键词
        {TokenType::If, "keywords: if"},
        {TokenType::Else, "keywords: else"},
        {TokenType::Int, "keywords : int"},

        {TokenType::Identifier, "indentifier"},     //标识符
        {TokenType::IntLiteral, "intLiteral" },   //整型字面量
        {TokenType::StringLiteral, "stringLiteral"} //字符串字面量
};
