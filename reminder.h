//
// Created by Administrator on 2022/5/15 0015.
//

#ifndef PARSER_REMINDER_H
#define PARSER_REMINDER_H

#include "file_reader.h"
using namespace std;

class Reminder
{
private:
    std::ostream &err;

public:
    explicit Reminder(ostream &error_stream);
    void report(unsigned int line, const string &msg);
};

#endif //PARSER_REMINDER_H
