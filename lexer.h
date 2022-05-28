//
// Created by Administrator on 2022/5/15 0015.
//

#ifndef PARSER_LEXER_H
#define PARSER_LEXER_H

#include <iostream>
#include "token.h"
#include "reminder.h"
#include "file_reader.h"
#include "errkinds.h"

using namespace std;

// Get next char
#define getnext (last_char = reader.get_char())

// Read a character ahead of time
#define SEEK (reader.seek())

class lexer
{
private:
    string ident;
    FileReader &reader;
public:
    Reminder &reminder;

    explicit lexer(FileReader &reader, Reminder &reminder):reader(reader),reminder(reminder){};

    // get next token.
    Token &get_token();

    int line(){
        return reader.getLineno();
    }
    int col(){
        return reader.getColno();
    }

    void logerr(const string & msg){
        reminder.report(reader.getLineno(),msg);
    }
};

#endif //PARSER_LEXER_H
