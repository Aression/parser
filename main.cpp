#include <bits/stdc++.h>
#include "file_reader.h"
#include "lexer.h"
#include "parser.h"


int main(int argc, char const *argv[])
{
    FileReader reader(R"(D:\CPPWorkDIR\parser\testfile\testfile.txt)");
    lexer lex(&reader, cerr);
    std::ofstream out(R"(D:\CPPWorkDIR\parser\testfile\output.txt)");
    parser parse(lex,out);
    parse.parseProgram();
    return 0;
}
