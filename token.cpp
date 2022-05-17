//
// Created by Administrator on 2022/5/15 0015.
//
#include "token.h"
Token::Token(TokenType type, std::string literal) : type(type), literal(std::move(literal)) {}

std::string Token::get_type() {
    if (type == TokenType::IDENFR)
        return "IDENFR";
    else if (type == TokenType::INTCON)
        return "INTCON";
    else if (type == TokenType::CHARCON)
        return "CHARCON";
    else if (type == TokenType::STRCON)
        return "STRCON";
    else if (type == TokenType::CONSTTK)
        return "CONSTTK";
    else if (type == TokenType::INTTK)
        return "INTTK";
    else if (type == TokenType::CHARTK)
        return "CHARTK";
    else if (type == TokenType::VOIDTK)
        return "VOIDTK";
    else if (type == TokenType::MAINTK)
        return "MAINTK";
    else if (type == TokenType::IFTK)
        return "IFTK";
    else if (type == TokenType::ELSETK)
        return "ELSETK";
    else if (type == TokenType::DOTK)
        return "DOTK";
    else if (type == TokenType::SWITCHTK)
        return "SWITCHTK";
    else if (type == TokenType::CASETK)
        return "CASETK";
    else if (type == TokenType::DEFAULTTK)
        return "DEFAULTTK";
    else if (type == TokenType::WHILETK)
        return "WHILETK";
    else if (type == TokenType::FORTK)
        return "FORTK";
    else if (type == TokenType::SCANFTK)
        return "SCANFTK";
    else if (type == TokenType::PRINTFTK)
        return "PRINTFTK";
    else if (type == TokenType::RETURNTK)
        return "RETURNTK";
    else if (type == TokenType::PLUS)
        return "PLUS";
    else if (type == TokenType::MINU)
        return "MINU";
    else if (type == TokenType::MULT)
        return "MULT";
    else if (type == TokenType::DIV)
        return "DIV";
    else if (type == TokenType::LSS)
        return "LSS";
    else if (type == TokenType::LEQ)
        return "LEQ";
    else if (type == TokenType::GRE)
        return "GRE";
    else if (type == TokenType::GEQ)
        return "GEQ";
    else if (type == TokenType::EQL)
        return "EQL";
    else if (type == TokenType::NEQ)
        return "NEQ";
    else if (type == TokenType::COLON){
        return "COLON";
    }else if (type == TokenType::ASSIGN)
        return "ASSIGN";
    else if (type == TokenType::SEMICN)
        return "SEMICN";
    else if (type == TokenType::COMMA)
        return "COMMA";
    else if (type == TokenType::LPARENT)
        return "LPARENT";
    else if (type == TokenType::RPARENT)
        return "RPARENT";
    else if (type == TokenType::LBRACK)
        return "LBRACK";
    else if (type == TokenType::RBRACK)
        return "RBRACK";
    else if (type == TokenType::LBRACE)
        return "LBRACE";
    else if (type == TokenType::RBRACE)
        return "RBRACE";
    else
        return "UNREGONIZED";
}
