//
// Created by Administrator on 2022/5/15 0015.
//
#include "reminder.h"

void Reminder::report(unsigned int line, unsigned int col, const string &msg, const string &prefix){
    err << "[" << prefix << "] at position:" << line << ":" << col << " " << msg << endl;
}

Reminder::Reminder(ostream &error_stream) : err(error_stream) {
}

void Reminder::logerr(const FileReader *reader, const string &msg) {
    report(reader->getLineno(), reader->getColno(), msg, "ERROR");
}

void Reminder::logwarn(const FileReader *reader, const string &msg) {
    report(reader->getLineno(), reader->getColno(), msg, "WARNING");
}
