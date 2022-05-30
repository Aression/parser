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

    pair<int,int> position = make_pair(line(),col());

    // 如果遇到文件的末尾
    if (last_char == EOF) {
        token = Token(TokenType::EOFTOK, "");
        token.position=position;
        return token;
    }

    if(last_char == '_'){
        ident = last_char;
        while (isalnum(last_char = reader.get_char()) || last_char == '_') {
            ident += last_char;
        }
        token = Token(TokenType::IDENFR, ident);
        token.position=position;
        return token;
    }

    // 判断identifier和keyword
    if (isalpha(last_char)) {
        ident = last_char;
        while (isalnum(last_char = reader.get_char()) || last_char == '_') {
            ident += last_char;
        }
        if (cmp_str_i(ident,"const")) {
            token = Token(TokenType::CONSTTK, ident);
            token.position=position;
            return token;
        } else if (cmp_str_i(ident,"int")) {
            token = Token(TokenType::INTTK, ident);
            token.position=position;
            return token;
        } else if (cmp_str_i(ident,"char")) {
            token = Token(TokenType::CHARTK, ident);
            token.position=position;
            return token;
        } else if (cmp_str_i(ident,"void")) {
            token = Token(TokenType::VOIDTK, ident);
            token.position=position;
            return token;
        } else if (cmp_str_i(ident,"main")) {
            token = Token(TokenType::MAINTK, ident);
            token.position=position;
            return token;
        } else if (cmp_str_i(ident,"if")) {
            token = Token(TokenType::IFTK, ident);
            token.position=position;
            return token;
        } else if (cmp_str_i(ident,"else")) {
            token = Token(TokenType::ELSETK, ident);
            token.position=position;
            return token;
        }else if (cmp_str_i(ident,"switch")){
            token = Token(TokenType::SWITCHTK, ident);
            token.position=position;
            return token;
        } else if (cmp_str_i(ident,"case")){
            token = Token(TokenType::CASETK, ident);
            token.position=position;
            return token;
        } else if (cmp_str_i(ident,"default")){
            token = Token(TokenType::DEFAULTTK, ident);
            token.position=position;
            return token;
        }else if (cmp_str_i(ident,"while")) {
            token = Token(TokenType::WHILETK, ident);
            token.position=position;
            return token;
        } else if (cmp_str_i(ident,"for")) {
            token = Token(TokenType::FORTK, ident);
            token.position=position;
            return token;
        } else if (cmp_str_i(ident,"scanf")) {
            token = Token(TokenType::SCANFTK, ident);
            token.position=position;
            return token;
        } else if (cmp_str_i(ident,"printf")) {
            token = Token(TokenType::PRINTFTK, ident);
            token.position=position;
            return token;
        } else if (cmp_str_i(ident,"return")) {
            token = Token(TokenType::RETURNTK, ident);
            token.position=position;
            return token;
        } else {
            token = Token(TokenType::IDENFR, ident);
            token.position=position;
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
        token.position=position;
        return token;
    }

    // 判断字符串是否合法: 十进制编码为32,33,35-126的ASCII字符
    if (last_char == '\"') {
        std::string str;
        int strnormal=1, strcount=0;
        while ((last_char = reader.get_char()) != '\"') {
            if(!(last_char == 32 || last_char==33 || (last_char>=35 && last_char<=126))){
                strnormal=0;
            }
            strcount++;
            str += last_char;
        }
        getnext;
        //字符串中间有引号的情况不考虑
        if(strnormal==0 || strcount==0){
            //有非法字符或者为空字符串均为非法情况
            logerr(ERR::meaning(errs::a));
            token = Token(TokenType::UNREGONIZED, str);
            token.normalToken=false;
        }else{
            token = Token(TokenType::STRCON, str);
        }
        token.position=position;
        return token;
    }
        //判断字符异常情况
    else if (last_char == '\'') {
        int count = 0;//记录被单引号包裹的字符的数量,如果大于1的话就报错
        std::string str;
        int normalchar=1;
        while ((last_char = reader.get_char()) != '\'') {
            if(!(isalnum(last_char) || last_char=='+'||last_char=='-' || last_char=='*'||last_char=='/' || last_char == '_')){
                normalchar=0;
            }
            str += last_char;
            count++;
        }
        if(normalchar==0 || count != 1){
            logerr(ERR::meaning(errs::a));//非法字符
            token = Token(TokenType::CHARCON, str);
            token.normalToken=false;
        }else {//正常的
            token = Token(TokenType::CHARCON, str);
        }
        getnext;
        token.position=position;
        return token;
    }
    //判断特殊符号
    if (last_char == '+') {
        token = Token(TokenType::PLUS, "+");
        getnext;
        token.position=position;
        return token;
    } else if (last_char == '-') {
        token = Token(TokenType::MINU, "-");
        getnext;
        token.position=position;
        return token;
    } else if (last_char == '*') {
        token = Token(TokenType::MULT, "*");
        getnext;
        token.position=position;
        return token;
    } else if (last_char == '/') {
        token = Token(TokenType::DIV, "/");
        getnext;
        token.position=position;
        return token;
    } else if (last_char == '<') {
        if (SEEK == '=') {
            reader.get_char();
            getnext;
            token = Token(TokenType::LEQ, "<=");
            token.position=position;
            return token;
        }
        getnext;
        token = Token(TokenType::LSS, "<");
        token.position=position;
        return token;
    } else if (last_char == '>') {
        if (SEEK == '=') {
            reader.get_char();
            getnext;
            token = Token(TokenType::GEQ, ">=");
            token.position=position;
            return token;
        }
        getnext;
        token = Token(TokenType::GRE, ">");
        token.position=position;
        return token;

    } else if (last_char == '=') {
        if (SEEK == '=') {
            reader.get_char();
            getnext;
            token = Token(TokenType::EQL, "==");
            token.position=position;
            return token;
        }
        getnext;
        token = Token(TokenType::ASSIGN, "=");
        token.position=position;
        return token;

    } else if (last_char == '!') {
        if (SEEK == '=') {
            reader.get_char();
            getnext;
            token = Token(TokenType::NEQ, "!=");
            token.position=position;
            return token;
        } else {
            getnext;
            token = Token(TokenType::UNREGONIZED, "!");
            logerr(ERR::meaning(errs::a));//非法符号
            token.position=position;
            token.normalToken=false;
            return token;
        }
    }else if (last_char == ':') {
        getnext;
        token = Token(TokenType::COLON, ":");
        token.position=position;
        return token;
    }else if (last_char == ';') {
        getnext;
        token = Token(TokenType::SEMICN, ";");
        token.position=position;
        return token;
    } else if (last_char == ',') {
        getnext;
        token = Token(TokenType::COMMA, ",");
        token.position=position;
        return token;
    } else if (last_char == '(') {
        getnext;
        token = Token(TokenType::LPARENT, "(");
        token.position=position;
        return token;
    } else if (last_char == ')') {
        getnext;
        token = Token(TokenType::RPARENT, ")");
        token.position=position;
        return token;
    } else if (last_char == '[') {
        getnext;
        token = Token(TokenType::LBRACK, "[");
        token.position=position;
        return token;
    } else if (last_char == ']') {
        getnext;
        token = Token(TokenType::RBRACK, "]");
        token.position=position;
        return token;
    } else if (last_char == '{') {
        getnext;
        token = Token(TokenType::LBRACE, "{");
        token.position=position;
        return token;
    } else if (last_char == '}') {
        getnext;
        token = Token(TokenType::RBRACE, "}");
        token.position=position;
        return token;
    }
    logerr(ERR::meaning(errs::a));//未定义的字符
    std::string literal;
    literal+=last_char;
    token = Token(TokenType::UNREGONIZED,literal);
    token.position=position;
    token.normalToken=false;
    getnext;
    return token;
}