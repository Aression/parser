#include <bits/stdc++.h>
#include "file_reader.h"
#include "lexer.h"
#include "parser.h"
#include "symbol_table.h"


void test_symbol_table(){
    SymbolTable table;
    table.insert("a","char",0,0,1,1,vector<int>());
    table.insert("b","char",0,1,1,1,vector<int>());
    printf("variable b char islocal=%d\n",table.islocal("b","char"));

    table.loc();
    table.insert("a","char",0,0,1,1,vector<int>());
    table.insert("b","char",0,1,1,1,vector<int>());
    printf("variable b char islocal=%d\n",table.islocal("b","char"));
    table.reloc();

    table.loc();
    table.insert("d","char",0,1,1,1,vector<int>());
    printf("variable d char islocal=%d\n",table.islocal("d","char"));
    printf("variable b char islocal=%d\n",table.islocal("b","char"));
    table.reloc();

    printf("variable b char islocal=%d\n",table.islocal("b","char"));
}




int main(int argc, char const *argv[])
{
    ifstream fin(R"(D:\CPPWorkDIR\parser\testfile\testfile.txt)");
    ofstream fout(R"(D:\CPPWorkDIR\parser\testfile\output.txt)");

    //读取器和错误记录模块
    FileReader reader(fin);
    Reminder reminder(fout);

    //编译器执行逻辑：一遍式，先扫描token，生成语法树和创建符号表，再进行语义分析和翻译，完成目标代码生成。
    lexer lex(reader, reminder);
    parser parse(lex,fout);
    parse.parseProgram();
    return 0;
}
