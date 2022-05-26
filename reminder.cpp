//
// Created by Administrator on 2022/5/15 0015.
//
#include "reminder.h"

void Reminder::report(unsigned int line, const string &msg) {
    err  << line << " " << msg << endl;
}

Reminder::Reminder(ostream &error_stream) : err(error_stream) {
}

void Reminder::logerr(const FileReader & reader, const string & msg){
    report(reader.getLineno(), msg);
}