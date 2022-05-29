#include <bits/stdc++.h>
#include "file_reader.h"
#include "lexer.h"
#include "parser.h"
#include "symbol_table.h"


int main(int argc, char const *argv[])
{
    ifstream fin(R"(testfile.txt)");
    ofstream fout(R"(error.txt)");

    //读取器和错误记录模块
    FileReader reader(fin);
    Reminder reminder(fout);

    //编译器执行逻辑：一遍式，先扫描token，生成语法树和创建符号表，再进行语义分析和翻译，完成目标代码生成。
    lexer lex(reader, reminder);
    parser parse(lex,fout);
    parse.parseProgram();
    return 0;
}
