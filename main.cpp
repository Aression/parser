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
    FileReader reader(R"(E:\Workplace2\parser\testfile\testfile.txt)");
    lexer lex(&reader, cerr);
    std::ofstream out(R"(E:\Workplace2\parser\testfile\output.txt)");
    parser parse(lex,out);
    parse.parseProgram();
    return 0;
}
