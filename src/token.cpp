//
// Created by Frank on 2020-04-03.
//

#include "token.hpp"

const unordered_map<enum TokenType, string> Token::kwTypeMap_ {
        {TokenType::If, "IF"},
        {TokenType::Else, "ELSE"},
        {TokenType::Int, "INT"},
};

const unordered_map<TokenType, string> Token::tagLexemeMap_{
        {TokenType::Plus, "operator< + >"},   // +
        {TokenType::Minus, "operator< - >"}, // -
        {TokenType::Star, "operator< * >"},  // *
        {TokenType::Slash, "operator< / >"}, //
        {TokenType::GE, "operator< >= >"},     // >=
        {TokenType::GT, "operator< > >"},     //
        {TokenType::EQ, "operator< == >"},    // ==
        {TokenType::LE, "operator< <= >"},     // <=
        {TokenType::LT, "operator< < >"},     // <
        {TokenType::Assignment, "operator< = >"},// =

        // delimiter 限界符
        {TokenType::SemiColon, "delimiter< ; >"}, // ;
        {TokenType::LeftParen, "delimiter< ( >"}, // (
        {TokenType::RightParen, "delimiter< ) >"},// )
        {TokenType::BigLeftParen, "delimiter< { >"}, // (
        {TokenType::BigRightParen, "delimiter< } >"},// )

        // 关键词
        {TokenType::If, "keywords<if>"},
        {TokenType::Else, "keywords<else>"},
        {TokenType::Int, "keywords<int>"},
        {TokenType::While, "keywords<while>"},
        {TokenType::Float, "keywords<float>"},
        {TokenType::Return, "keywords<return>"},
        {TokenType::Break, "keywords<break>"},
        {TokenType::Main, "keywords<main>"},

        {TokenType::Identifier, "indentifier"},     //标识符
        {TokenType::IntLiteral, "intLiteral" },   //整型字面量
        {TokenType::FloatLiteral, "floatLiteral" },   //浮点型字面量
        {TokenType::ScienceLiteral, "scienceLiteral" },   //科学计数法
        {TokenType::StringLiteral, "stringLiteral"}, //字符串字面量
        {TokenType::CharLiteral, "CharLiteral"}, //字符字面量

        // 进制
        {TokenType::Binary, "Binary"},     //二进制
        {TokenType::Octal, "Octal"},     //八进制
        {TokenType::HexDecimal, "HexDecimal"},     //十六进制


};

