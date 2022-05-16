//
// Created by Administrator on 2022/5/15 0015.
//

#include <iostream>
#include "utility"

#ifndef PARSER_TOKEN_H
#define PARSER_TOKEN_H

enum TokenType {
    EOFTOK = 0,// 文件结束
    IDENFR,// 标识符
    INTCON,// 整数
    CHARCON,// 字符
    STRCON,// 字符串
    CONSTTK,// const
    INTTK,// int
    CHARTK,// char
    VOIDTK,// void
    MAINTK,// main
    IFTK,// if
    ELSETK,// else
    DOTK,// do
    WHILETK,// while
    FORTK,// for
    SCANFTK,// scanf
    PRINTFTK,// printf
    RETURNTK,// return
    PLUS,// +
    MINU,// -
    MULT,// *
    DIV,// /
    LSS,// <
    LEQ,// <=
    GRE,// >
    GEQ,// >=
    EQL,// ==
    NEQ,// !=
    ASSIGN,// =
    SEMICN,// ;
    COMMA,// ,
    LPARENT,// (
    RPARENT,// )
    LBRACK,// [
    RBRACK,// ]
    LBRACE,// {
    RBRACE,  // }
    UNREGONIZED //无法被识别
};


/*
usage:
1. include: token.h
2. use: Token tk = Token(TokenType::[type], [literal]);
*/
class Token {
public:
    Token(TokenType type, std::string literal);

    // token的属性类型
    std::string literal; // token的值
    TokenType type;
    std::string get_type();
};

#endif //PARSER_TOKEN_H
