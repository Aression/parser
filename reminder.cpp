//
// Created by Administrator on 2022/5/15 0015.
//
#include "reminder.h"

void Reminder::report(unsigned int line, const string &msg) {
    err  << line+1 << " " << msg << endl;//line从1开始
}

Reminder::Reminder(ostream &error_stream) : err(error_stream) {
}