//
// Created by Administrator on 2022/5/15 0015.
//
#include "lexer.h"
#include <algorithm>

// unrecognized token
static Token token(TokenType::UNREGONIZED, "");

string strToLower(const string& strA) {
    string strTmp = strA;
    transform(strTmp.begin(), strTmp.end(), strTmp.begin(), ::tolower);
    return strTmp;
}

bool cmp_str_i(string a, string b) {
    string str1 = strToLower(a);
    string str2 = strToLower(b);
    return (str1 == str2);
}

Token &lexer::get_token(){
    static char last_char = ' ';
    while(isspace(last_char)){
        getnext;
    }

    // 如果遇到文件的末尾 或者已经停止获取了
    if (last_char == EOF || !normal) {
        token = Token(TokenType::EOFTOK, "");
        return token;
    }

    if(last_char == '_'){
        ident = last_char;
        while (isalnum(last_char = reader->get_char()) || last_char == '_') {
            ident += last_char;
        }
        token = Token(TokenType::IDENFR, ident);
        return token;
    }

    // 判断identifier和keyword
    if (isalpha(last_char)) {
        ident = last_char;
        while (isalnum(last_char = reader->get_char()) || last_char == '_') {
            ident += last_char;
        }
        if (cmp_str_i(ident,"const")) {
            token = Token(TokenType::CONSTTK, ident);
            return token;
        } else if (cmp_str_i(ident,"int")) {
            token = Token(TokenType::INTTK, ident);
            return token;
        } else if (cmp_str_i(ident,"char")) {
            token = Token(TokenType::CHARTK, ident);
            return token;
        } else if (cmp_str_i(ident,"void")) {
            token = Token(TokenType::VOIDTK, ident);
            return token;
        } else if (cmp_str_i(ident,"main")) {
            token = Token(TokenType::MAINTK, ident);
            return token;
        } else if (cmp_str_i(ident,"if")) {
            token = Token(TokenType::IFTK, ident);
            return token;
        } else if (cmp_str_i(ident,"else")) {
            token = Token(TokenType::ELSETK, ident);
            return token;
        } else if (cmp_str_i(ident,"do")) {
            token = Token(TokenType::DOTK, ident);
            return token;
        } else if (cmp_str_i(ident,"switch")){
            token = Token(TokenType::SWITCHTK, ident);
            return token;
        } else if (cmp_str_i(ident,"case")){
            token = Token(TokenType::CASETK, ident);
            return token;
        } else if (cmp_str_i(ident,"default")){
            token = Token(TokenType::DEFAULTTK, ident);
            return token;
        }else if (cmp_str_i(ident,"while")) {
            token = Token(TokenType::WHILETK, ident);
            return token;
        } else if (cmp_str_i(ident,"for")) {
            token = Token(TokenType::FORTK, ident);
            return token;
        } else if (cmp_str_i(ident,"scanf")) {
            token = Token(TokenType::SCANFTK, ident);
            return token;
        } else if (cmp_str_i(ident,"printf")) {
            token = Token(TokenType::PRINTFTK, ident);
            return token;
        } else if (cmp_str_i(ident,"return")) {
            token = Token(TokenType::RETURNTK, ident);
            return token;
        } else {
            token = Token(TokenType::IDENFR, ident);
            return token;
        }
    }
    // 判断数字
    if (isdigit(last_char)) {
        std::string num_str;
        do {
            num_str += (char) last_char;
            getnext;
        } while (isdigit(last_char));
        token = Token(TokenType::INTCON, num_str);
        return token;
    }

    // 判断字符串
    if (last_char == '\"') {
        std::string str;
        while ((last_char = reader->get_char()) != '\"') {
            str += last_char;
        }
        getnext;
        //todo 字符串中间可能会含有引号
        token = Token(TokenType::STRCON, str);
        return token;
    }
        //判断字符
    else if (last_char == '\'') {
        int count = 0;//记录被单引号包裹的字符的数量,如果大于1的话就报错
        std::string str;
        while ((last_char = reader->get_char()) != '\'') {
            str += last_char;
            count++;
        }
        if (count != 1) {
            // reminder->logwarn(reader,"illegal char");
        }
        getnext;
        token = Token(TokenType::CHARCON, str);
        return token;
    }
    //判断特殊符号
    if (last_char == '+') {
        token = Token(TokenType::PLUS, "+");
        getnext;
        return token;
    } else if (last_char == '-') {
        token = Token(TokenType::MINU, "-");
        getnext;
        return token;
    } else if (last_char == '*') {
        token = Token(TokenType::MULT, "*");
        getnext;
        return token;
    } else if (last_char == '/') {
        token = Token(TokenType::DIV, "/");
        getnext;
        return token;
    } else if (last_char == '<') {
        if (SEEK == '=') {
            reader->get_char();
            getnext;
            token = Token(TokenType::LEQ, "<=");
            return token;
        }
        getnext;
        token = Token(TokenType::LSS, "<");
        return token;
    } else if (last_char == '>') {
        if (SEEK == '=') {
            reader->get_char();
            getnext;
            token = Token(TokenType::GEQ, ">=");
            return token;
        }
        getnext;
        token = Token(TokenType::GRE, ">");
        return token;

    } else if (last_char == '=') {
        if (SEEK == '=') {
            reader->get_char();
            getnext;
            token = Token(TokenType::EQL, "==");
            return token;
        }
        getnext;
        token = Token(TokenType::ASSIGN, "=");
        return token;

    } else if (last_char == '!') {
        if (SEEK == '=') {
            reader->get_char();
            getnext;
            token = Token(TokenType::NEQ, "!=");
            return token;
        } else {
            // todo 需要进行错误处理
            getnext;
            token = Token(TokenType::UNREGONIZED, "!");
            return token;
        }
    }else if (last_char == ':') {
        getnext;
        token = Token(TokenType::COLON, ":");
        return token;
    }else if (last_char == ';') {
        getnext;
        token = Token(TokenType::SEMICN, ";");
        return token;
    } else if (last_char == ',') {
        getnext;
        token = Token(TokenType::COMMA, ",");
        return token;
    } else if (last_char == '(') {
        getnext;
        token = Token(TokenType::LPARENT, "(");
        return token;
    } else if (last_char == ')') {
        getnext;
        token = Token(TokenType::RPARENT, ")");
        return token;
    } else if (last_char == '[') {
        getnext;
        token = Token(TokenType::LBRACK, "[");
        return token;
    } else if (last_char == ']') {
        getnext;
        token = Token(TokenType::RBRACK, "]");
        return token;
    } else if (last_char == '{') {
        getnext;
        token = Token(TokenType::LBRACE, "{");
        return token;
    } else if (last_char == '}') {
        getnext;
        token = Token(TokenType::RBRACE, "}");
        return token;
    }

    string unrecognized = "X";
    unrecognized[0]=last_char;
    token = Token(TokenType::UNREGONIZED, unrecognized);
    err("Unrecognized char: ["+unrecognized+"]");

    getnext;
    return token;
}

void lexer::err(const string& msg) {
    reminder->logerr(reader,msg);
    normal=0;
}

void lexer::warn(const string& msg) {
    reminder->logwarn(reader,msg);
}
