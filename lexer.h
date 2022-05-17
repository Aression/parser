//
// Created by Administrator on 2022/5/15 0015.
//

#include <iostream>

#ifndef PARSER_LEXER_H
#define PARSER_LEXER_H

#include "token.h"
#include "reminder.h"
#include "file_reader.h"
using namespace std;

// Get next char
#define getnext (last_char = reader->get_char())

// Read a character ahead of time
#define SEEK (reader->seek())

class lexer
{
private:
    string ident;
    FileReader *reader;
    Reminder *reminder;
    int normal;
public:
    explicit lexer(FileReader *reader, ostream &err):reader(reader){
        reminder = new Reminder(err);
        normal=1;
    }

    // get next token.
    Token &get_token();

    // record error
    void err(const string& msg);

    // record warn
    void warn(const string& msg);
};

#endif //PARSER_LEXER_H
